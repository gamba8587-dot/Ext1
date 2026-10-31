#include <utilities/memory/memory.hpp>
#include <utilities/addresses/addresses.hpp>
#include <core/features/features.hpp>
#include "../systems.hpp"
namespace systems {
	bool events::initialize( )
	{
		if ( !register_listener( "bullet_impact", [ ]( void* event ) { features::misc::g_impacts.on_bullet_impact( reinterpret_cast< std::uintptr_t >( event ) ); } ) )
		{
			return false;
		}
		if ( !register_listener( "player_hurt", [ ]( void* event ) { features::misc::g_impacts.on_player_hurt( reinterpret_cast< std::uintptr_t >( event ) ); } ) )
		{
			return false;
		}
		if ( !register_listener( "round_start", [ ]( void* event ) { features::misc::g_other.on_round_start( ); } ) )
		{
			return false;
		}
		return true;
	}
	void events::shutdown( )
	{
		for ( auto& entry : m_listeners )
		{
			if ( entry->registered )
			{
				memory::call_vfunc<void>( addresses::globals::game_event_manager, 5, &entry->listener );
				entry->registered = false;
			}
		}
		m_listeners.clear( );
	}
	bool events::register_listener( const char* event_name, handler_fn handler )
	{
		if ( !event_name || !handler )
		{
			return false;
		}
		auto current_entry = std::make_unique<entry>( );
		current_entry->handler = handler;
		current_entry->name = event_name;
		current_entry->registered = false;
		current_entry->vtable_data[ 0 ] = nullptr;
		current_entry->vtable_data[ 1 ] = reinterpret_cast< void* >( &fire_event );
		current_entry->vtable_data[ 2 ] = reinterpret_cast< void* >( &get_debug_id );
		current_entry->listener.vtable = current_entry->vtable_data;
		current_entry->listener.debug_id = static_cast< int >( m_listeners.size( ) + 1 );
		const auto success = memory::call_vfunc<bool>( addresses::globals::game_event_manager, 3, &current_entry->listener, event_name, false );
		if ( !success )
		{
			return false;
		}
		current_entry->registered = true;
		m_listeners.push_back( std::move( current_entry ) );
		return true;
	}
	void* __fastcall events::fire_event( void* self, void* event )
	{
		const auto current_listener = reinterpret_cast< listener* >( self );
		for ( const auto& entry : m_listeners )
		{
			if ( entry->listener.debug_id == current_listener->debug_id && entry->handler )
			{
				entry->handler( event );
				break;
			}
		}
		return nullptr;
	}
	int __fastcall events::get_debug_id( void* self )
	{
		const auto current_listener = reinterpret_cast< listener* >( self );
		return current_listener->debug_id;
	}
}

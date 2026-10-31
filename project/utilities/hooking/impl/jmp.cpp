#include <Windows.h>
#include <external/minhook/minhook.h>
#include <utilities/logging/logging.hpp>
#include "../hooking.hpp"
namespace hooking {
	namespace {
		volatile LONG g_minhook_state{};
		bool ensure_minhook_initialized( )
		{
			auto state = InterlockedCompareExchange( &g_minhook_state, 1, 0 );
			if ( state == 0 )
			{
				const auto status = MH_Initialize( );
				InterlockedExchange( &g_minhook_state, status == MH_OK || status == MH_ERROR_ALREADY_INITIALIZED ? 2 : -1 );
				return g_minhook_state == 2;
			}
			while ( ( state = g_minhook_state ) == 1 )
				SwitchToThread( );
			return state == 2;
		}
	}
	bool jmp::create( void* target, void* hook_function )
	{
		if ( this->is_valid( ) )
			return true;
		if ( !target || !hook_function || !ensure_minhook_initialized( ) )
			return false;
		void* trampoline{};
		const auto status = MH_CreateHook( target, hook_function, &trampoline );
		if ( status != MH_OK || !trampoline )
		{
			logging::console::print( "MinHook create failed: status {} at {:X}", static_cast<int>( status ), reinterpret_cast<std::uintptr_t>( target ) );
			return false;
		}
		this->m_target = target;
		this->m_hook = hook_function;
		this->m_trampoline = trampoline;
		this->m_enabled = false;
		this->m_original_length = sizeof( this->m_original_bytes );
		std::memcpy( this->m_original_bytes, target, this->m_original_length );
		return true;
	}
	bool jmp::enable( )
	{
		if ( !this->is_valid( ) )
			return false;
		if ( this->m_enabled )
			return true;
		const auto status = MH_EnableHook( this->m_target );
		if ( status != MH_OK && status != MH_ERROR_ENABLED )
		{
			logging::console::print( "MinHook enable failed: status {} at {:X}", static_cast<int>( status ), reinterpret_cast<std::uintptr_t>( this->m_target ) );
			return false;
		}
		this->m_enabled = true;
		return true;
	}
	bool jmp::disable( )
	{
		if ( !this->is_valid( ) )
			return false;
		if ( !this->m_enabled )
			return true;
		const auto status = MH_DisableHook( this->m_target );
		if ( status != MH_OK && status != MH_ERROR_DISABLED )
			return false;
		this->m_enabled = false;
		return true;
	}
	void jmp::reset( )
	{
		if ( !this->is_valid( ) )
			return;
		const auto status = MH_RemoveHook( this->m_target );
		if ( status != MH_OK && status != MH_ERROR_NOT_CREATED )
			logging::console::print( "MinHook remove failed: status {} at {:X}", static_cast<int>( status ), reinterpret_cast<std::uintptr_t>( this->m_target ) );
		this->m_target = nullptr;
		this->m_hook = nullptr;
		this->m_trampoline = nullptr;
		this->m_original_length = 0;
		this->m_enabled = false;
		std::memset( this->m_original_bytes, 0, sizeof( this->m_original_bytes ) );
	}
}

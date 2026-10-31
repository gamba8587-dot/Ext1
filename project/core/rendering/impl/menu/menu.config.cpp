#include <utilities/math/math.hpp>
#include <core/settings.hpp>
#include "../../rendering.hpp"
namespace rendering {
	namespace detail {
		std::string search_buf{};
		std::vector<std::wstring> config_list{};
		auto selected{ -1 };
		auto needs_refresh{ true };
		auto confirm_delete{ false };
		auto confirm_reset{ false };
		auto confirm_timer{ 0.0f };
		static inline void wide_to_utf8( const std::wstring& wide, char* out, int out_size )
		{
			WideCharToMultiByte( CP_UTF8, 0, wide.c_str( ), -1, out, out_size, nullptr, nullptr );
		}
		static inline std::wstring utf8_to_wide( const std::string& utf8 )
		{
			wchar_t buf[ 128 ]{};
			MultiByteToWideChar( CP_UTF8, 0, utf8.c_str( ), -1, buf, 128 );
			return buf;
		}
		static inline bool config_matches_search( const std::wstring& wname )
		{
			if ( detail::search_buf.empty( ) )
			{
				return true;
			}
			char narrow[ 128 ]{};
			wide_to_utf8( wname, narrow, sizeof( narrow ) );
			std::string lower_name{ narrow };
			std::string lower_search{ detail::search_buf };
			for ( auto& c : lower_name )
			{
				c = static_cast< char >( std::tolower( c ) );
			}
			for ( auto& c : lower_search )
			{
				c = static_cast< char >( std::tolower( c ) );
			}
			return lower_name.find( lower_search ) != std::string::npos;
		}
		static inline std::string selected_name( )
		{
			if ( detail::selected < 0 || detail::selected >= static_cast< int >( detail::config_list.size( ) ) )
			{
				return {};
			}
			char narrow[ 128 ]{};
			wide_to_utf8( detail::config_list[ detail::selected ], narrow, sizeof( narrow ) );
			return narrow;
		}
		static inline std::string config_date( const std::wstring& name )
		{
			WIN32_FILE_ATTRIBUTE_DATA data{};
			if ( !GetFileAttributesExW( config::registry::file_path( name ).c_str( ), GetFileExInfoStandard, &data ) )
				return "";
			FILETIME local_time{};
			SYSTEMTIME date{};
			if ( !FileTimeToLocalFileTime( &data.ftLastWriteTime, &local_time ) || !FileTimeToSystemTime( &local_time, &date ) )
				return "";
			constexpr const char* months[]{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
			if ( date.wMonth < 1 || date.wMonth > 12 ) return "";
			char text[ 32 ]{};
			std::snprintf( text, sizeof( text ), "%u %s %u", date.wDay, months[ date.wMonth - 1 ], date.wYear );
			return text;
		}
		static inline void reset_defaults( )
		{
			auto& reg = config::detail::get_registry( );
			for ( auto& f : reg.fields )
			{
				char key_str[ 12 ];
				std::snprintf( key_str, sizeof( key_str ), "%08x", f.key );
				auto def = reg.defaults.find( key_str );
				if ( def != reg.defaults.end( ) )
				{
					config::serial::json_to_field( *def, f );
				}
			}
			settings::finalize_binds( );
		}
	}
	void menu::draw_config( float group_w )
	{
		( void )group_w;
		if ( detail::needs_refresh )
		{
			detail::config_list = config::registry::list( );
			detail::needs_refresh = false;
			if ( detail::selected >= static_cast< int >( detail::config_list.size( ) ) )
			{
				detail::selected = -1;
			}
		}
		const auto dt = xdraw::delta_time( );
		if ( detail::confirm_delete || detail::confirm_reset )
		{
			detail::confirm_timer += dt;
			if ( detail::confirm_timer > 3.0f )
			{
				detail::confirm_delete = false;
				detail::confirm_reset = false;
			}
		}
		auto& dl = xui::draw::current( );
		const auto& s = xui::ctx( ).style;
		const auto& input = xui::ctx( ).input;
		const auto panel_x = this->m_body_x;
		const auto panel_y = this->m_body_y;
		const auto panel_w = this->m_body_w;
		const auto panel_h = this->m_body_h;
		dl.rect_filled( panel_x, panel_y, panel_w, panel_h, tokens::col_card, xdraw::corner_radius{ tokens::card_rounding } );
		dl.rect( panel_x, panel_y, panel_w, panel_h, xdraw::color{ 59, 55, 75, 255 }, xdraw::corner_radius{ tokens::card_rounding } );
		dl.text( panel_x + 22.0f, panel_y + 22.0f, this->m_workshop_configs ? "Workshop Configs" : "My Configs", tokens::col_text );
		if ( this->m_workshop_configs )
		{
			dl.text( panel_x + 22.0f, panel_y + 72.0f, "No workshop configs are available.", tokens::col_text_dim );
			return;
		}
		const auto inner_x = panel_x + 20.0f;
		const auto input_y = panel_y + 58.0f;
		constexpr auto action_w = 82.0f;
		constexpr auto action_gap = 10.0f;
		const auto input_w = panel_w - 40.0f - action_w * 2.0f - action_gap * 2.0f;
		xui::layout::set_cursor( inner_x - this->m_x, input_y - this->m_y );
		if ( xui::begin_child( "##cfg_name_box", input_w, 46.0f, false ) )
		{
			xui::text_input( "##cfg_name", detail::search_buf, 64, "Config Name..." );
			xui::end_child( );
		}
		xui::layout::set_cursor( inner_x + input_w + action_gap - this->m_x, input_y - this->m_y );
		xui::push_style_color( xui::style_col::button_bg, xdraw::color{ 87, 72, 168, 255 } );
		xui::push_style_color( xui::style_col::button_border, tokens::col_accent );
		xui::push_style_color( xui::style_col::text_dim, tokens::col_text );
		if ( xui::button( "Save", action_w, 46.0f ) && !detail::search_buf.empty( ) )
		{
			config::registry::save( detail::utf8_to_wide( detail::search_buf ) );
			detail::needs_refresh = true;
		}
		xui::pop_style_color( 3 );
		xui::layout::set_cursor( inner_x + input_w + action_gap * 2.0f + action_w - this->m_x, input_y - this->m_y );
		xui::push_style_color( xui::style_col::button_bg, tokens::col_elevated );
		xui::push_style_color( xui::style_col::button_border, xdraw::color{ 83, 79, 99, 255 } );
		if ( detail::confirm_delete )
		{
			if ( xui::button( "Confirm", action_w, 46.0f ) && detail::selected >= 0 && detail::selected < static_cast<int>( detail::config_list.size( ) ) )
			{
				config::registry::remove( detail::config_list[ detail::selected ] );
				detail::selected = -1;
				detail::needs_refresh = true;
				detail::confirm_delete = false;
			}
		}
		else if ( xui::button( "Delete", action_w, 46.0f ) && detail::selected >= 0 )
		{
			detail::confirm_delete = true;
			detail::confirm_timer = 0.0f;
		}
		xui::pop_style_color( 2 );
		dl.line( inner_x, panel_y + 121.0f, panel_x + panel_w - 20.0f, panel_y + 121.0f,
			xdraw::color{ 59, 55, 75, 255 } );
		dl.text( inner_x, panel_y + 134.0f, "SAVED CONFIGS", tokens::col_text_dim );
		const auto list_y = panel_y + 157.0f;
		const auto list_h = panel_h - ( list_y - panel_y ) - 16.0f;
		xui::layout::set_cursor( inner_x - this->m_x, list_y - this->m_y );
		xui::push_style_color( xui::style_col::child_bg, tokens::col_card );
		xui::push_style_color( xui::style_col::child_border, xdraw::color{ 0, 0, 0, 0 } );
		if ( xui::begin_child( "##cfg_cards", panel_w - 40.0f, list_h, true ) )
		{
			const auto row_w = xui::layout::avail( ).first;
			for ( auto i = 0; i < static_cast<int>( detail::config_list.size( ) ); ++i )
			{
				const auto& name = detail::config_list[ i ];
				char narrow[ 128 ]{};
				detail::wide_to_utf8( name, narrow, sizeof( narrow ) );
				const auto row = xui::layout::item( row_w, 80.0f );
				const auto is_selected = detail::selected == i;
				dl.rect_filled( row.x, row.y, row.w, row.h, is_selected ? xdraw::color{ 32, 28, 49, 255 } : tokens::col_elevated, xdraw::corner_radius{ tokens::btn_rounding } );
				dl.rect( row.x, row.y, row.w, row.h, is_selected ? tokens::col_accent : xdraw::color{ 67, 63, 88, 255 }, xdraw::corner_radius{ tokens::btn_rounding } );
				dl.rect_filled( row.x, row.y + 12.0f, 3.0f, row.h - 24.0f, tokens::col_accent );
				dl.text( row.x + 20.0f, row.y + 17.0f, narrow, tokens::col_text );
				const auto detail_line = std::string( "Original    |    " ) + detail::config_date( name );
				dl.text( row.x + 20.0f, row.y + 47.0f, detail_line, tokens::col_text_dim );
				const auto edit = xui::rect{ row.x + row.w - 112.0f, row.y + 20.0f, 43.0f, 40.0f };
				const auto save = xui::rect{ row.x + row.w - 59.0f, row.y + 20.0f, 45.0f, 40.0f };
				dl.text( edit.x, edit.y + 9.0f, "Edit", tokens::col_accent );
				dl.text( save.x, save.y + 9.0f, "Save", tokens::col_accent );
				if ( input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
				{
					if ( input.in_rect( edit ) || input.in_rect( row ) && !input.in_rect( save ) )
					{
						detail::selected = i;
						detail::search_buf = narrow;
						config::registry::load( name );
						settings::finalize_binds( );
						 detail::confirm_delete = false;
					}
					else if ( input.in_rect( save ) )
					{
						config::registry::save( name );
						 detail::needs_refresh = true;
					}
				}
			}
			if ( detail::config_list.empty( ) )
			{
				dl.text( inner_x + 20.0f, list_y + 25.0f, "No configs yet. Enter a name and select Save.", tokens::col_text_dim );
			}
			xui::end_child( );
		}
		xui::pop_style_color( 2 );

	}
}

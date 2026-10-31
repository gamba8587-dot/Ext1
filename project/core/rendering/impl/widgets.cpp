#include <utilities/math/math.hpp>
#include <utilities/memory/memory.hpp>
#include <utilities/addresses/addresses.hpp>
#include <core/systems/systems.hpp>
#include <core/settings.hpp>
#include <core/features/features.hpp>
#include <core/rendering/overlay_star.hpp>
#include "../rendering.hpp"
#include <utilities/security/security.hpp>
namespace rendering {
	namespace {
		struct overlay_position
		{
			float x{}, y{}, grab_x{}, grab_y{};
			bool initialized{}, dragging{};
		};
		overlay_position* active_overlay{};

		void update_overlay_position( overlay_position& position, float default_x, float default_y,
			float width, float height, float grab_height )
		{
			const auto [screen_w, screen_h] = xdraw::viewport_size( );
			if ( !position.initialized )
			{
				position.x = default_x;
				position.y = default_y;
				position.initialized = true;
			}
			auto& input = xui::ctx( ).input;
			if ( !g_menu.is_open( ) || !input.mouse_down )
			{
				position.dragging = false;
				if ( active_overlay == &position ) active_overlay = nullptr;
			}
			if ( g_menu.is_open( ) && active_overlay == nullptr && input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) &&
				input.mouse_x >= position.x && input.mouse_x < position.x + width &&
				input.mouse_y >= position.y && input.mouse_y < position.y + grab_height )
			{
				position.dragging = true;
				active_overlay = &position;
				position.grab_x = input.mouse_x - position.x;
				position.grab_y = input.mouse_y - position.y;
			}
			if ( position.dragging )
			{
				position.x = input.mouse_x - position.grab_x;
				position.y = input.mouse_y - position.grab_y;
			}
			position.x = std::clamp( position.x, 8.0f, std::max( 8.0f, static_cast<float>( screen_w ) - width - 8.0f ) );
			position.y = std::clamp( position.y, 8.0f, std::max( 8.0f, static_cast<float>( screen_h ) - height - 8.0f ) );
		}
	}
	void widgets::draw( )
	{
		if ( !g_menu.is_open( ) || !xui::ctx( ).input.mouse_down )
			active_overlay = nullptr;
		auto& dl = xdraw::get( );
		if ( settings::g_misc.m_watermark.enabled.value )
		{
			this->watermark( dl );
		}
		if ( settings::g_misc.keybinds_enabled.value )
		{
			this->keybinds( dl );
		}
	}
	void widgets::watermark( xdraw::draw_list& draw_list )
	{
		const auto screen_w = xdraw::viewport_size( ).first;
		const auto& s  = xui::ctx( ).style;
		const auto& wm = settings::g_misc.m_watermark;
		const auto framerate = xdraw::framerate( );
		const auto local = systems::g_local.get( );
		constexpr auto h{ 32.0f };
		constexpr auto margin{ 92.0f };
		SYSTEMTIME st{};
		GetLocalTime( &st );
		char time_buf[ 8 ]{};
		std::snprintf( time_buf, sizeof( time_buf ), "%02d:%02d", st.wHour, st.wMinute );
		static auto smoothed_fps{ 0.0f };
		if ( smoothed_fps == 0.0f ) smoothed_fps = framerate;
		smoothed_fps += ( framerate - smoothed_fps ) * std::min( 2.0f * xdraw::delta_time( ), 1.0f );
		char fps_val[ 8 ]{};
		std::snprintf( fps_val, sizeof( fps_val ), "%.0f", smoothed_fps );
		auto ping{ 0 };
		if ( local.is_alive && local.controller && systems::g_entities.exists( local.controller ) )
			ping = memory::read<std::uint32_t>( local.controller + SCHEMA( "CCSPlayerController", "m_iPing"_hash ) );
		char ping_val[ 8 ]{};
		std::snprintf( ping_val, sizeof( ping_val ), "%d", ping );
		const bool has_map = wm.show_map.value && !s_map_name.empty( );
		static auto last_server_tick{ 0 };
		static auto last_curtime{ 0.0f };
		static auto measured_tickrate{ 0 };
		if ( local.controller )
		{
			const auto net_for_tick = addresses::globals::network_client_service;
			const auto tick_state   = net_for_tick ? memory::call_vfunc<std::uintptr_t>( net_for_tick, 23 ) : 0;
			const auto server_tick  = tick_state   ? memory::read<int>( tick_state + 892 ) : 0;
			const auto gv           = memory::read<std::uintptr_t>( addresses::globals::global_vars );
			const auto curtime      = gv ? memory::read<float>( gv + 0x30 ) : 0.0f;
			if ( server_tick > 0 && last_server_tick > 0 && curtime - last_curtime >= 2.0f )
			{
				const auto tick_delta = server_tick - last_server_tick;
				const auto time_delta = curtime - last_curtime;
				if ( tick_delta > 0 && time_delta > 0.5f )
				{
					const auto rate = static_cast<int>( std::round( tick_delta / time_delta ) );
					if ( rate >= 16 && rate <= 256 ) measured_tickrate = rate;
				}
				last_server_tick = server_tick;
				last_curtime     = curtime;
			}
			else if ( last_server_tick == 0 && server_tick > 0 )
			{
				last_server_tick = server_tick;
				last_curtime     = curtime;
			}
		}
		else
		{
			last_server_tick = 0;
			last_curtime     = 0.0f;
			measured_tickrate = 0;
		}
		const bool has_tick = wm.show_tick.value && local.controller && measured_tickrate > 0;
		char tick_val[ 8 ]{};
		if ( has_tick ) std::snprintf( tick_val, sizeof( tick_val ), "%d", measured_tickrate );
		const bool has_velocity = wm.show_velocity.value && local.is_alive && local.pawn;
		static auto smoothed_velocity{ 0.0f };
		char vel_val[ 8 ]{};
		if ( has_velocity )
		{
			const auto velocity = memory::read<math::vector3>( local.pawn + SCHEMA( "C_BaseEntity", "m_vecAbsVelocity"_hash ) );
			const auto speed = velocity.length_2d( );
			smoothed_velocity += ( speed - smoothed_velocity ) * std::min( 8.0f * xdraw::delta_time( ), 1.0f );
			std::snprintf( vel_val, sizeof( vel_val ), "%.0f", smoothed_velocity );
		}
		else
		{
			smoothed_velocity = 0.0f;
		}
		struct segment { std::string value; std::string unit; float width{}; };
		std::vector<segment> segments{};
		if ( wm.show_fps.value ) segments.push_back( { fps_val, " fps" } );
		if ( wm.show_ping.value ) segments.push_back( { ping_val, " ms" } );
		if ( wm.show_time.value ) segments.push_back( { time_buf, "" } );
		if ( wm.show_user.value ) segments.push_back( { "developer", "" } );
		if ( has_map ) segments.push_back( { s_map_name, "" } );
		if ( has_tick ) segments.push_back( { tick_val, " tick" } );
		if ( has_velocity ) segments.push_back( { vel_val, " u/s" } );
		const auto [name_w, name_h] = xdraw::measure_text( "Celestial" );
		float width = 38.0f + name_w + 10.0f;
		const auto max_width = std::max( 120.0f, std::min( 420.0f, static_cast<float>( screen_w ) - 24.0f ) );
		std::size_t visible_count{};
		for ( auto& part : segments )
		{
			part.width = xdraw::measure_text( part.value ).first + xdraw::measure_text( part.unit ).first;
			if ( width + 17.0f + part.width + 10.0f > max_width ) break;
			width += 17.0f + part.width;
			++visible_count;
		}
		width += 10.0f;
		static overlay_position position{};
		update_overlay_position( position, static_cast<float>( screen_w ) - width - margin, 74.0f, width, h, h );
		const auto x = position.x;
		const auto y = position.y;
		const auto radius = xdraw::corner_radius{ 6.0f };
		draw_list.rect_filled( x, y, width, h, xdraw::color{ 20, 19, 27, 235 }, radius );
		draw_list.rect( x, y, width, h, xdraw::color{ 83, 78, 102, 220 }, radius );
		draw_overlay_star( draw_list, x + 20.0f, y + h * 0.5f, s.accent );
		draw_list.text( x + 38.0f, y + ( h - name_h ) * 0.5f, "Celestial", s.text );
		auto cursor = x + 38.0f + name_w + 10.0f;
		for ( std::size_t i = 0; i < visible_count; ++i )
		{
			const auto& part = segments[ i ];
			draw_list.line( cursor + 5.0f, y + 9.0f, cursor + 5.0f, y + h - 9.0f, xdraw::color{ 79, 75, 95, 255 } );
			cursor += 17.0f;
			const auto [value_w, value_h] = xdraw::measure_text( part.value );
			draw_list.text( cursor, y + ( h - value_h ) * 0.5f, part.value, s.accent );
			const auto unit_h = xdraw::measure_text( part.unit ).second;
			if ( !part.unit.empty( ) ) draw_list.text( cursor + value_w, y + ( h - unit_h ) * 0.5f, part.unit, s.text_dim );
			cursor += part.width;
		}

	}
	void widgets::keybinds( xdraw::draw_list& draw_list )
	{
		struct row_anim_t
		{
			animation::fade alpha;
			animation::spring offset_y;
			bool active_this_frame{ false };
			bool visible_target{ false };
		};
		static std::map<std::string, row_anim_t> row_states;
		static animation::fade container_alpha;
		const auto screen_w = xdraw::viewport_size( ).first;
		const auto& s = xui::ctx( ).style;
		constexpr auto margin{ 49.0f };
		constexpr auto row_spacing{ 2.0f };
		constexpr auto row_h{ 24.0f };
		constexpr auto header_h{ 32.0f };
		constexpr auto text_nudge{ 0.5f };
		struct bind_entry
		{
			const char* name;
			xui::bind_mode mode;
		};
		bind_entry entries[ 32 ]{};
		auto count{ 0 };
		const auto& ctx = features::combat::g_shared.ctx( );
		const auto has_weapon = ctx.valid && ctx.weapon_type >= cstypes::weapon_type::pistol && ctx.weapon_type <= cstypes::weapon_type::lmg;
		for ( const auto setting : xui::binds::all( ) )
		{
			if ( !setting || setting->bind.key == 0 || !setting->bind.active || count >= 32 )
			{
				continue;
			}
			auto is_rage_group{ false };
			for ( auto i = 0u; i < settings::combat::ragebot::k_group_count; ++i )
			{
				const auto& g = settings::g_combat.m_ragebot.groups[ i ];
				if ( setting == &g.min_damage_override || setting == &g.hitchance_override )
				{
					is_rage_group = true;
					break;
				}
			}
			if ( is_rage_group )
			{
				if ( !settings::g_combat.m_ragebot.enabled || !has_weapon )
				{
					continue;
				}
				const auto active_group = &settings::g_combat.m_ragebot.get_group( ctx.weapon_type );
				auto is_active{ false };
				for ( auto i = 0u; i < settings::combat::ragebot::k_group_count; ++i )
				{
					const auto& g = settings::g_combat.m_ragebot.groups[ i ];
					if ( &g == active_group )
					{
						if ( setting == &g.min_damage_override || setting == &g.hitchance_override )
						{
							is_active = true;
						}
						break;
					}
				}
				if ( !is_active )
				{
					continue;
				}
				auto& e = entries[ count++ ];
				e.name = setting->name.c_str( );
				e.mode = setting->bind.mode;
				continue;
			}
			auto is_legit_group{ false };
			for ( auto i = 0u; i < settings::combat::legitbot::k_group_count; ++i )
			{
				const auto& g = settings::g_combat.m_legitbot.groups[ i ];
				if ( setting == &g.aimbot || setting == &g.rcs || setting == &g.standalone_rcs || setting == &g.triggerbot || setting == &g.autowall || setting == &g.visualize_fov || setting == &g.trigger_head_only || setting == &g.give_me_your_seed )
				{
					is_legit_group = true;
					break;
				}
			}
			if ( is_legit_group )
			{
				if ( !settings::g_combat.m_legitbot.enabled.value || !has_weapon )
				{
					continue;
				}
				const auto* active_group = &settings::g_combat.m_legitbot.get_group( ctx.weapon_type );
				auto is_active{ false };
				for ( auto i = 0u; i < settings::combat::legitbot::k_group_count; ++i )
				{
					if ( &settings::g_combat.m_legitbot.groups[ i ] == active_group )
					{
						const auto& g = settings::g_combat.m_legitbot.groups[ i ];
						if ( setting == &g.aimbot || setting == &g.rcs || setting == &g.standalone_rcs || setting == &g.triggerbot || setting == &g.autowall || setting == &g.visualize_fov || setting == &g.trigger_head_only || setting == &g.give_me_your_seed )
						{
							is_active = true;
						}
						if ( is_active && setting == &active_group->give_me_your_seed && !active_group->triggerbot.value )
						{
							is_active = false;
						}
						break;
					}
				}
				if ( !is_active )
				{
					continue;
				}
				auto& e = entries[ count++ ];
				e.name = setting->name.c_str( );
				e.mode = setting->bind.mode;
				continue;
			}
			if ( setting == &settings::g_combat.m_antiaim.enabled || setting == &settings::g_combat.m_antiaim.manual_left || setting == &settings::g_combat.m_antiaim.manual_right || setting == &settings::g_combat.m_antiaim.hide_shots || setting == &settings::g_combat.m_antiaim.avoid_backstab || setting == &settings::g_combat.m_antiaim.direction_indicator )
			{
				if ( !settings::g_combat.m_antiaim.enabled.value )
				{
					continue;
				}
			}
			auto& e = entries[ count++ ];
			e.name = setting->name.c_str( );
			e.mode = setting->bind.mode;
		}
		// Only restart the fade when the desired state actually changes. Calling
		// fade_out() every frame re-runs tween::start(), which resets elapsed and
		// pins the value just above zero forever, so the header never disappears.
		{
			static bool shown{ false };
			const auto want = count > 0;
			if ( want != shown )
			{
				if ( want )
				{
					container_alpha.fade_in( 0.2f );
				}
				else
				{
					container_alpha.fade_out( 0.2f );
				}
				shown = want;
			}
		}
		container_alpha.update( );
		if ( !container_alpha.visible( ) )
			return;
		const auto master_alpha = container_alpha.alpha( );
		const auto target_base_y = 292.0f;
		static overlay_position position{};
		constexpr auto title = "Celestial Keybinds";
		const auto header_th = xdraw::measure_text( title ).second;
		const auto header_w = 248.0f;
		const auto panel_h = header_h + 6.0f + count * ( row_h + row_spacing );
		update_overlay_position( position, static_cast<float>( screen_w ) - header_w - margin,
			target_base_y, header_w, panel_h, header_h );
		const auto x = position.x;
		const auto base_ry = position.y;
		const auto master_u8 = static_cast< std::uint8_t >( 255.0f * master_alpha );
		draw_list.rect_filled( x, base_ry, header_w, header_h,
			xdraw::color{ 20, 19, 27, static_cast< std::uint8_t >( 235.0f * master_alpha ) }, xdraw::corner_radius{ 6.0f } );
		draw_list.rect( x, base_ry, header_w, header_h,
			xdraw::color{ 83, 78, 102, static_cast< std::uint8_t >( 220.0f * master_alpha ) }, xdraw::corner_radius{ 6.0f } );
		const auto star_x = x + 20.0f;
		const auto star_y = base_ry + header_h * 0.5f;
		draw_overlay_star( draw_list, star_x, star_y, s.accent.alpha( master_u8 ) );
		draw_list.text(
			x + 38.0f,
			base_ry + ( header_h - header_th ) * 0.5f + text_nudge,
			title,
			xdraw::color{ 244, 242, 250, static_cast< std::uint8_t >( 255.0f * master_alpha ) } );
		for ( auto& [name, state] : row_states )
			state.active_this_frame = false;
		float current_offset_y = header_h + 6.0f;
		for ( auto i = 0; i < count; ++i )
		{
			const auto& e = entries[ i ];
			auto& anim = row_states[ e.name ];
			if ( !anim.active_this_frame && anim.alpha.alpha( ) <= 0.01f )
				anim.offset_y.snap( current_offset_y );
			anim.active_this_frame = true;
			if ( !anim.visible_target )
			{
				anim.alpha.fade_in( 0.2f );
				anim.visible_target = true;
			}
			anim.offset_y.set_target( current_offset_y );
			anim.alpha.update( );
			anim.offset_y.update( );
			const auto row_alpha = anim.alpha.alpha( ) * master_alpha;
			const auto draw_y = base_ry + anim.offset_y.value( );
			const auto row_u8 = static_cast< std::uint8_t >( 255.0f * row_alpha );
			const auto is_toggle = ( e.mode == xui::bind_mode::toggle );
			{
				const auto mode = is_toggle ? "TOGGLE" : "HOLD";
				const auto [mode_w, mode_h] = xdraw::measure_text( mode );
				std::string name{ e.name };
				const auto max_name_w = header_w - mode_w - 32.0f;
				if ( xdraw::measure_text( name ).first > max_name_w )
				{
					while ( !name.empty( ) && xdraw::measure_text( name + "..." ).first > max_name_w ) name.pop_back( );
					name += "...";
				}
				const auto name_h = xdraw::measure_text( name ).second;
				draw_list.text( x + 8.0f, draw_y + ( row_h - name_h ) * 0.5f + text_nudge, name,
					xdraw::color{ 244, 242, 250, row_u8 }, xdraw::text_style::shadowed );
				draw_list.text( x + header_w - mode_w - 8.0f, draw_y + ( row_h - mode_h ) * 0.5f, mode,
					s.accent.alpha( row_u8 ), xdraw::text_style::shadowed );
			}
			current_offset_y += row_h + row_spacing;
		}
		for ( auto it = row_states.begin( ); it != row_states.end( ); )
		{
			if ( !it->second.active_this_frame )
			{
				if ( it->second.visible_target )
				{
					it->second.alpha.fade_out( 0.15f );
					it->second.visible_target = false;
				}
				it->second.alpha.update( );
				it->second.offset_y.update( );
				if ( it->second.alpha.alpha( ) <= 0.001f )
				{
					it = row_states.erase( it );
					continue;
				}
			}
			++it;
		}
	}
}

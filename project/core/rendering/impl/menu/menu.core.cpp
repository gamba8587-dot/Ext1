#include <utilities/memory/memory.hpp>
#include <utilities/addresses/addresses.hpp>
#include <core/settings.hpp>
#include <utilities/security/security.hpp>
#include <external/config.hpp>
#include <core/systems/systems.hpp>
#include <core/resources/icons/menu_tabs.hpp>
#include "../../rendering.hpp"
namespace rendering {
	namespace svgs {
		constexpr auto star = R"(<svg width="32" height="32" viewBox="0 0 32 32" xmlns="http://www.w3.org/2000/svg"><path d="M16 0 18.6 11.5 29.8 2.2 20.5 13.4 32 16 20.5 18.6 29.8 29.8 18.6 20.5 16 32 13.4 20.5 2.2 29.8 11.5 18.6 0 16 11.5 13.4 2.2 2.2 13.4 11.5Z" fill="#9F8BFF"/></svg>)";
		constexpr auto logo = R"(<svg width="17" height="14" viewBox="0 0 17 14" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M0.148138 10.5306L7.19953 0.643266C7.81122 -0.214422 9.08773 -0.214422 9.69942 0.643266L16.7254 10.4949C17.099 11.0189 16.7236 11.7453 16.0792 11.7453H14.5344C14.1378 11.7453 13.7815 11.5036 13.6355 11.1356L11.5458 5.86857C11.4326 5.58318 11.1562 5.3957 10.8486 5.3957C10.7131 5.3957 10.6149 5.52457 10.651 5.65485L12.6899 12.9972C12.8299 13.5014 12.4499 14 11.9256 14H10.5367C10.2323 14 9.97322 13.7786 9.92648 13.4784L8.68012 4.66254C8.64757 4.45362 8.28767 4.45728 8.24676 4.66475L6.57384 13.5028C6.51683 13.7917 6.26301 14 5.96795 14H4.58215C4.04293 14 3.66094 13.4746 3.8282 12.9631L6.17936 5.7721C6.23588 5.59921 6.10678 5.4216 5.92452 5.4216H5.87377C5.57672 5.4216 5.30764 5.59658 5.1877 5.86781L2.81532 11.2326C2.65733 11.5899 2.29903 11.8169 1.9078 11.8076L0.775512 11.7808C0.139818 11.7656 -0.220464 11.0476 0.148214 10.5306H0.148138Z" fill="#111111"/> </svg>)";
		constexpr auto tab_rage = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M3.75 12C3.75 13.0834 3.96339 14.1562 4.37799 15.1571C4.7926 16.1581 5.40029 17.0675 6.16637 17.8336C6.93245 18.5997 7.84193 19.2074 8.84286 19.622C9.8438 20.0366 10.9166 20.25 12 20.25M20.25 12C20.25 10.9166 20.0366 9.8438 19.622 8.84286C19.2074 7.84193 18.5997 6.93245 17.8336 6.16637C17.0675 5.40029 16.1581 4.7926 15.1571 4.37799C14.1562 3.96339 13.0834 3.75 12 3.75C10.9166 3.75 9.8438 3.96339 8.84286 4.37799C7.84193 4.7926 6.93245 5.40029 6.16637 6.16637C5.40029 6.93245 4.7926 7.84193 4.37799 8.84286C3.96339 9.8438 3.75 10.9166 3.75 12M3.75 12H7.41667M12 20.25C13.0834 20.25 14.1562 20.0366 15.1571 19.622C16.1581 19.2074 17.0675 18.5997 17.8336 17.8336C18.5997 17.0675 19.2074 16.1581 19.622 15.1571C20.0366 14.1562 20.25 13.0834 20.25 12M12 20.25V17.5M20.25 12H17.5M12 3.75V7.41667M12 12V12.01" stroke="white" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/></svg>)";
		constexpr auto tab_legit = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill="white" d="M6 8h5.25V2h-.25C8.24 2 6 4.24 6 7v1Zm0 1.5V13c0 3.31 2.24 5.5 5 5.5h2c2.76 0 5-2.19 5-5.5V9.5H6ZM18 8V7c0-2.76-2.24-5-5-5h-.25v6H18Z"/></svg>)";
		constexpr auto tab_player = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M12 3.8C9.902 3.8 8.2 5.502 8.2 7.6C8.2 9.698 9.902 11.4 12 11.4C14.098 11.4 15.8 9.698 15.8 7.6C15.8 5.502 14.098 3.8 12 3.8Z" fill="white"/><path d="M4 20.1C4 16.565 6.865 13.7 10.4 13.7H13.6C17.135 13.7 20 16.565 20 20.1V20.2C20 20.752 19.552 21.2 19 21.2H5C4.448 21.2 4 20.752 4 20.2V20.1Z" fill="white"/></svg>)";
		constexpr auto tab_world = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill="white" d="M14.8 13.2H9.2c.1 2 .5 3.8 1.2 5.2.3.7.7 1.3 1 1.6.3.3.5.3.7.3s.4 0 .7-.3c.3-.3.7-.9 1-1.6.5-1.4.9-3.2 1-5.2ZM9.2 10.8h5.6c-.1-2-.5-3.8-1.2-5.2-.3-.7-.7-1.3-1-1.6-.3-.3-.5-.3-.7-.3s-.4 0-.7.3c-.3.3-.7.9-1 1.6-.5 1.4-.9 3.2-1 5.2ZM7.8 10.8c.1-2.6.7-5 1.7-6.8-3 1-5.1 3.7-5.4 6.8h3.7ZM4.1 13.2c.3 3.1 2.4 5.8 5.4 6.8-1-1.8-1.6-4.2-1.7-6.8H4.1ZM16.2 13.2c-.1 2.6-.7 5-1.7 6.8 3-1 5.1-3.7 5.4-6.8h-3.7ZM19.9 10.8c-.3-3.1-2.4-5.8-5.4-6.8 1 1.8 1.6 4.2 1.7 6.8h3.7Z"/></svg>)";
		constexpr auto tab_skins = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M4.61 7.09L11.00 3.97C11.64 3.65 12.37 3.65 13.01 3.97L19.40 7.09C20.15 7.46 20.15 8.11 19.40 8.48L13.20 11.58C12.55 11.91 11.80 12.00 11.13 11.67L4.61 8.56C3.86 8.20 3.86 7.46 4.61 7.09Z" fill="white"/><path d="M4.61 10.90L5.38 10.52L11.02 13.27C11.65 13.59 12.38 13.66 13.02 13.33L18.27 10.74L19.40 11.30C20.15 11.67 20.15 12.33 19.40 12.70L13.20 15.80C12.55 16.13 11.80 16.21 11.13 15.89L4.61 12.79C3.86 12.42 3.86 11.27 4.61 10.90Z" fill="white"/><path d="M4.61 15.04L5.38 14.66L11.02 17.41C11.65 17.73 12.38 17.80 13.02 17.47L18.27 14.88L19.40 15.44C20.15 15.81 20.15 16.47 19.40 16.84L13.20 19.95C12.55 20.28 11.80 20.36 11.13 20.03L4.61 16.93C3.86 16.56 3.86 15.41 4.61 15.04Z" fill="white"/></svg>)";
		constexpr auto tab_misc = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill-rule="evenodd" clip-rule="evenodd" d="M13.8286 2.15224C13.4583 2 12.9888 2 12.0498 2C11.1108 2 10.6413 2 10.271 2.15224C9.77722 2.35523 9.38489 2.74458 9.18036 3.23463C9.08699 3.45834 9.05045 3.7185 9.03615 4.09799C9.01514 4.65568 8.72696 5.17189 8.23997 5.45093C7.75298 5.72996 7.15844 5.71954 6.66129 5.45876C6.32298 5.2813 6.07769 5.18262 5.83579 5.15102C5.30589 5.08178 4.76998 5.22429 4.34596 5.5472C4.02794 5.78938 3.79319 6.1929 3.3237 6.99993C2.85421 7.80697 2.61947 8.21048 2.56715 8.60491C2.49738 9.1308 2.64098 9.66266 2.96635 10.0835C3.11486 10.2756 3.32357 10.437 3.6475 10.639C4.12371 10.936 4.43012 11.4419 4.43009 12C4.43006 12.5581 4.12366 13.0639 3.6475 13.3608C3.32352 13.5629 3.11477 13.7244 2.96625 13.9165C2.64088 14.3373 2.49729 14.8691 2.56705 15.395C2.61937 15.7894 2.85412 16.193 3.3236 17C3.79309 17.807 4.02784 18.2106 4.34586 18.4527C4.76988 18.7756 5.30579 18.9181 5.83569 18.8489C6.07758 18.8173 6.32285 18.7186 6.66113 18.5412C7.15832 18.2804 7.7529 18.27 8.23992 18.549C8.72694 18.8281 9.01513 19.3443 9.03615 19.9021C9.05045 20.2815 9.08699 20.5417 9.18036 20.7654C9.38489 21.2554 9.77722 21.6448 10.271 21.8478C10.6413 22 11.1108 22 12.0498 22C12.9888 22 13.4583 22 13.8286 21.8478C14.3224 21.6448 14.7147 21.2554 14.9192 20.7654C15.0126 20.5417 15.0492 20.2815 15.0635 19.902C15.0845 19.3443 15.3726 18.8281 15.8596 18.549C16.3466 18.2699 16.9412 18.2804 17.4384 18.5412C17.7767 18.7186 18.0219 18.8172 18.2638 18.8488C18.7937 18.9181 19.3296 18.7756 19.7536 18.4527C20.0717 18.2105 20.3064 17.807 20.7759 16.9999C21.2454 16.1929 21.4801 15.7894 21.5325 15.395C21.6022 14.8691 21.4586 14.3372 21.1333 13.9164C20.9847 13.7243 20.776 13.5628 20.452 13.3608C19.9759 13.0639 19.6695 12.558 19.6695 11.9999C19.6695 11.4418 19.9759 10.9361 20.4521 10.6392C20.7761 10.4371 20.9848 10.2757 21.1334 10.0835C21.4587 9.66273 21.6023 9.13087 21.5326 8.60497C21.4802 8.21055 21.2455 7.80703 20.776 7C20.3065 6.19297 20.0718 5.78945 19.7537 5.54727C19.3297 5.22436 18.7938 5.08185 18.2639 5.15109C18.022 5.18269 17.7767 5.28136 17.4385 5.4588C16.9413 5.71959 16.3467 5.73002 15.8597 5.45096C15.3727 5.17191 15.0845 4.65566 15.0635 4.09794C15.0492 3.71848 15.0126 3.45833 14.9192 3.23463C14.7147 2.74458 14.3224 2.35523 13.8286 2.15224ZM12.0498 15C13.7193 15 15.0726 13.6569 15.0726 12C15.0726 10.3431 13.7193 9 12.0498 9C10.3803 9 9.02696 10.3431 9.02696 12C9.02696 13.6569 10.3803 15 12.0498 15Z" fill="white"/></svg>)";
		constexpr auto tab_config = R"(<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M4.25 7.25C4.25 6.42157 4.92157 5.75 5.75 5.75H9L11 7.75H18.25C19.0784 7.75 19.75 8.42157 19.75 9.25V17.25C19.75 18.0784 19.0784 18.75 18.25 18.75H5.75C4.92157 18.75 4.25 18.0784 4.25 17.25V7.25Z" fill="white"/></svg>)";
		constexpr auto cfg_folder_black = R"(<svg width="15" height="13" viewBox="0 0 15 13" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M2.25 0.75H5.25L7.5 3H12.75C13.1478 3 13.5294 3.15804 13.8107 3.43934C14.092 3.72064 14.25 4.10218 14.25 4.5V10.5C14.25 10.8978 14.092 11.2794 13.8107 11.5607C13.5294 11.842 13.1478 12 12.75 12H2.25C1.85218 12 1.47064 11.842 1.18934 11.5607C0.908035 11.2794 0.75 10.8978 0.75 10.5V2.25C0.75 1.85218 0.908035 1.47064 1.18934 1.18934C1.47064 0.908035 1.85218 0.75 2.25 0.75Z" stroke="#111111" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/></svg>)";
		constexpr auto cfg_folder_dim = R"(<svg width="15" height="13" viewBox="0 0 15 13" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M2.25 0.75H5.25L7.5 3H12.75C13.1478 3 13.5294 3.15804 13.8107 3.43934C14.092 3.72064 14.25 4.10218 14.25 4.5V10.5C14.25 10.8978 14.092 11.2794 13.8107 11.5607C13.5294 11.842 13.1478 12 12.75 12H2.25C1.85218 12 1.47064 11.842 1.18934 11.5607C0.908035 11.2794 0.75 10.8978 0.75 10.5V2.25C0.75 1.85218 0.908035 1.47064 1.18934 1.18934C1.47064 0.908035 1.85218 0.75 2.25 0.75Z" stroke="#ADC0FF" stroke-opacity="0.32" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/></svg>)";
		constexpr auto cfg_cloud_black = R"(<svg width="17" height="12" viewBox="0 0 17 12" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M4.24275 10.4998C2.31375 10.4998 0.75 8.99454 0.75 7.13754C0.75 5.28129 2.31375 3.77604 4.24275 3.77604C4.5375 2.45454 5.58825 1.37604 6.999 0.946291C8.409 0.517291 9.966 0.801541 11.082 1.69629C12.198 2.58879 12.7035 3.95154 12.4095 5.27304H13.152C14.5867 5.27304 15.75 6.44304 15.75 7.88754C15.75 9.33279 14.5867 10.5028 13.1512 10.5028H4.24275" stroke="#111111" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/></svg>)";
		constexpr auto cfg_cloud_dim = R"(<svg width="17" height="12" viewBox="0 0 17 12" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M4.24275 10.4998C2.31375 10.4998 0.75 8.99454 0.75 7.13754C0.75 5.28129 2.31375 3.77604 4.24275 3.77604C4.5375 2.45454 5.58825 1.37604 6.999 0.946291C8.409 0.517291 9.966 0.801541 11.082 1.69629C12.198 2.58879 12.7035 3.95154 12.4095 5.27304H13.152C14.5867 5.27304 15.75 6.44304 15.75 7.88754C15.75 9.33279 14.5867 10.5028 13.1512 10.5028H4.24275" stroke="#ADC0FF" stroke-opacity="0.32" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/></svg>)";
		constexpr auto cfg_plus = R"(<svg width="16" height="15" viewBox="0 0 16 15" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M8 4V11M4 7.5H12" stroke="#ADC0FF" stroke-width="1.5" stroke-linecap="round"/></svg>)";
	}
	void menu::initialize_graphics( )
	{
		if ( this->m_graphics_ready )
		{
			return;
		}
		this->m_textures.logo.resource = xdraw::load_svg( svgs::star, 1.0f, &this->m_textures.logo.width, &this->m_textures.logo.height );
		constexpr float k_cfg_icon_scale{ 1.0f };
		this->m_textures.cfg_folder_on.resource = xdraw::load_svg( svgs::cfg_folder_black, k_cfg_icon_scale, &this->m_textures.cfg_folder_on.width, &this->m_textures.cfg_folder_on.height );
		this->m_textures.cfg_folder_off.resource = xdraw::load_svg( svgs::cfg_folder_dim, k_cfg_icon_scale, &this->m_textures.cfg_folder_off.width, &this->m_textures.cfg_folder_off.height );
		this->m_textures.cfg_cloud_on.resource = xdraw::load_svg( svgs::cfg_cloud_black, k_cfg_icon_scale, &this->m_textures.cfg_cloud_on.width, &this->m_textures.cfg_cloud_on.height );
		this->m_textures.cfg_cloud_off.resource = xdraw::load_svg( svgs::cfg_cloud_dim, k_cfg_icon_scale, &this->m_textures.cfg_cloud_off.width, &this->m_textures.cfg_cloud_off.height );
		this->m_textures.cfg_plus.resource = xdraw::load_svg( svgs::cfg_plus, 1.0f, &this->m_textures.cfg_plus.width, &this->m_textures.cfg_plus.height );
		struct icon_bytes { const unsigned char* data; std::size_t size; };
		constexpr std::array<icon_bytes, 7> tab_icons{{
			{ menu_icons::rage, sizeof( menu_icons::rage ) },
			{ menu_icons::legit, sizeof( menu_icons::legit ) },
			{ menu_icons::visuals, sizeof( menu_icons::visuals ) },
			{ menu_icons::world, sizeof( menu_icons::world ) },
			{ menu_icons::skins, sizeof( menu_icons::skins ) },
			{ menu_icons::misc, sizeof( menu_icons::misc ) },
			{ menu_icons::configs, sizeof( menu_icons::configs ) }
		}};
		for ( auto i = 0; i < 7; ++i )
		{
			const auto& icon = tab_icons[ i ];
			this->m_textures.tabs[ i ].resource = xdraw::load_texture(
				std::span<const std::byte>{ reinterpret_cast<const std::byte*>( icon.data ), icon.size },
				&this->m_textures.tabs[ i ].width, &this->m_textures.tabs[ i ].height );
		}
		this->m_graphics_ready = true;
	}
	void menu::draw( )
	{
		{
			// Framerate-independent fade. raw speed*dt converges at a different rate
			// per framerate, which makes the transition visibly stutter on high fps.
			const auto dt = xdraw::delta_time( );
			const auto target = this->m_open ? 1.0f : 0.0f;
			const auto speed = this->m_open ? 20.0f : 26.0f;
			const auto blend = 1.0f - std::exp( -speed * dt );
			this->m_open_anim += ( target - this->m_open_anim ) * (std::min)( blend, 1.0f );
			if ( std::fabs( target - this->m_open_anim ) < 0.004f )
			{
				this->m_open_anim = target;
			}
		}
		if ( this->m_last_open != this->m_open )
		{
			if ( this->m_open )
			{
				this->m_saved_relative_mouse = memory::read<std::uint8_t>( addresses::globals::input_system + 84 );
				memory::call_vfunc<void>( addresses::globals::input_system, 76, false );
				this->apply_saved_cursor( );
			}
			else
			{
				POINT pt{};
				if ( GetCursorPos( &pt ) )
				{
					this->m_saved_cursor_x = pt.x;
					this->m_saved_cursor_y = pt.y;
					this->m_has_saved_cursor = true;
				}
				memory::call_vfunc<void>( addresses::globals::input_system, 76, this->m_saved_relative_mouse != 0 );
			}
			this->m_last_open = this->m_open;
		}
		if ( !g_context.ui_assets_ready( ) )
		{
			return;
		}
		if ( !this->m_initial_positioned )
		{
			const auto [screen_w, screen_h] = xdraw::viewport_size( );
			this->m_x = std::max( 0.0f, ( static_cast<float>( screen_w ) - this->m_w ) * 0.5f );
			this->m_y = std::max( 0.0f, ( static_cast<float>( screen_h ) - this->m_h ) * 0.5f );
			this->m_initial_positioned = true;
		}
		// xui::begin() also drives binds::process(), which is what advances bind
		// state for the whole cheat. It must run every frame, including while the
		// menu is closed, or keybinds stop working.
		xui::begin( );
		const auto fade = xui::ease::out_cubic( (std::clamp)( this->m_open_anim, 0.0f, 1.0f ) );
		// Keep drawing while the fade-out plays out, then stop entirely.
		if ( !this->m_open && fade <= 0.0f )
		{
			xui::end( );
			return;
		}
		// Scoped alpha on the draw list: every primitive emitted while it is
		// active is multiplied by it, so the fade covers the menu only and leaves
		// shared state (keybinds, watermark) untouched. No palette mutation.
		xui::draw::current( ).push_alpha( fade );
		this->sync_theme_style( );
		{
			// reveal drives the window background, border and blur inside
			// begin_window; the scoped alpha above covers everything else we draw.
			if ( !xui::begin_window( "##menu", this->m_x, this->m_y, this->m_w, this->m_h, false, 200.0f, 200.0f, fade ) )
			{
				xui::draw::current( ).pop_alpha( );
				xui::end( );
				return;
			}
			xdraw::push_font( rendering::g_fonts.inter_bold );
			auto& dl = xui::draw::current( );
			const auto wx = this->m_x;
			const auto wy = this->m_y;
			const auto ww = this->m_w;
			const auto wh = this->m_h;
			const auto sb_x = wx + tokens::gap;
			const auto sb_y = wy + tokens::gap;
			const auto sb_w = tokens::sidebar_w;
			const auto sb_h = wh - tokens::gap * 2.0f;
			dl.rect_filled( sb_x, sb_y, sb_w, sb_h, tokens::col_dark );
			this->draw_side_bar( sb_h );
			const auto content_x = sb_x + sb_w + tokens::gap;
			const auto content_y = sb_y;
			const auto content_w = ww - tokens::gap * 2.0f - sb_w - tokens::gap;
			const auto content_h = sb_h;
			this->draw_top_bar( content_w );
			const auto top_h = this->m_tab == static_cast< int >( tab::config ) ? 142.0f : tokens::subtab_bar_h;
			const auto body_y = content_y + top_h + tokens::gap;
			const auto body_h = content_h - top_h - tokens::gap;
			const auto col_w = ( content_w - tokens::gap ) * 0.5f;
			const auto left_x = content_x;
			const auto right_x = content_x + col_w + tokens::gap;
			this->m_body_x = content_x;
			this->m_body_y = body_y;
			this->m_body_w = content_w;
			this->m_body_h = body_h;
			xui::layout::set_cursor( content_x - wx, body_y - wy );
			switch ( this->m_tab )
			{
			case 0: this->draw_ragebot( col_w ); break;
			case 1: this->draw_legitbot( col_w ); break;
			case 2: this->draw_player( col_w ); break;
			case 3: this->draw_world( col_w ); break;
			case 4: this->draw_skins( col_w ); break;
			case 5: this->draw_misc( col_w ); break;
			case 6: this->draw_config( col_w ); break;
			}
			xui::end_window( );
			xdraw::pop_font( );
		}
		xui::draw::current( ).pop_alpha( );
		xui::end( );
	}
	void menu::shutdown( ) const
	{
		if ( this->m_open )
		{
			memory::call_vfunc<void>( addresses::globals::input_system, 76, this->m_saved_relative_mouse != 0 );
		}
	}
	void menu::apply_saved_cursor( )
	{
		if ( !this->m_has_saved_cursor )
		{
			return;
		}
		const auto vx = GetSystemMetrics( SM_XVIRTUALSCREEN );
		const auto vy = GetSystemMetrics( SM_YVIRTUALSCREEN );
		const auto vw = GetSystemMetrics( SM_CXVIRTUALSCREEN );
		const auto vh = GetSystemMetrics( SM_CYVIRTUALSCREEN );
		const auto x = std::clamp( this->m_saved_cursor_x, vx, vx + std::max( 1, vw ) - 1 );
		const auto y = std::clamp( this->m_saved_cursor_y, vy, vy + std::max( 1, vh ) - 1 );
		SetCursorPos( x, y );
		SetCursor( LoadCursor( nullptr, IDC_ARROW ) );
	}
	void menu::draw_side_bar( float h )
	{
		auto& dl = xui::draw::current( );
		const auto& input = xui::ctx( ).input;
		const auto sb_x = this->m_x + tokens::gap;
		const auto sb_y = this->m_y + tokens::gap;
		if ( this->m_textures.logo.resource )
			dl.image( sb_x + 5.0f, sb_y + 9.0f, 32.0f, 32.0f, this->m_textures.logo.resource.Get( ) );
		dl.text( sb_x + 42.0f, sb_y + 12.0f, "CELESTIAL", tokens::col_text );
		dl.text( sb_x + 42.0f, sb_y + 33.0f, "PRIVATE / V2", tokens::col_text_dim );
		dl.rect_filled( sb_x + 9.0f, sb_y + 59.0f, tokens::sidebar_w - 18.0f, 1.0f, xdraw::color{ 55, 52, 71, 255 } );
		const auto tabs_y = sb_y + 73.0f;
		constexpr const char* labels[]{ "Ragebot", "Legitbot", "Visuals", "World", "Skins", "Misc", "Configs" };
		constexpr auto tab_count{ 7 };
		for ( auto i = 0; i < tab_count; ++i )
		{
			const auto btn = xui::rect{ sb_x + 7.0f, tabs_y + i * 48.0f, tokens::sidebar_w - 14.0f, 42.0f };
			const auto hovered = input.in_rect( btn );
			const auto is_active = ( this->m_tab == i );
			if ( hovered && input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
			{
				this->m_tab = i;
				this->m_subtab = 0;
			}
			if ( is_active || hovered )
			{
				dl.rect_filled( btn.x, btn.y, btn.w, btn.h, is_active ? xdraw::color{ 45, 41, 72, 255 } : tokens::col_card, xdraw::corner_radius{ tokens::btn_rounding } );
				if ( is_active ) dl.rect_filled( btn.x, btn.y, 4.0f, btn.h, tokens::col_accent );
			}
			const auto icon_col = tokens::col_accent.alpha( is_active ? 255 : 178 );
			{
				const auto& tex = this->m_textures.tabs[ i ];
				const auto iw = static_cast< float >( tex.width );
				const auto ih = static_cast< float >( tex.height );
				const auto icon_x = btn.x + 8.0f;
				const auto icon_y = std::floor( btn.y + ( btn.h - ih ) * 0.5f );
				dl.image( icon_x, icon_y, iw, ih, tex.resource.Get( ), icon_col );
			}
			dl.text( btn.x + 40.0f, btn.y + 13.0f, labels[ i ], is_active ? tokens::col_text : tokens::col_text_dim );
		}
		dl.text( sb_x + 14.0f, sb_y + h - 33.0f, "* ONLINE", tokens::col_accent );
		dl.text( sb_x + 14.0f, sb_y + h - 17.0f, "Celestial | 2026", tokens::col_text_dim );
	}
	void menu::sync_theme_style( ) const
	{
		auto& style = xui::ctx( ).style;
		style.window_bg = tokens::col_elevated.alpha( 175 );
		style.child_bg = tokens::col_card;
		style.checkbox_bg = xdraw::color{ 34, 33, 43, 255 };
		style.checkbox_border = xdraw::color{ 130, 130, 142, 255 };
		style.checkbox_mark = tokens::col_accent;
		style.checkbox_mark_icon = tokens::col_dark;
		style.slider_track = tokens::col_card;
		style.slider_fill = tokens::col_accent;
		style.button_bg = tokens::col_card;
		style.button_hovered = tokens::col_card.alpha( 120 );
		style.button_active = tokens::col_accent;
		style.keybind_bg = tokens::col_card;
		style.keybind_waiting = tokens::col_accent;
		style.combo_bg = tokens::col_card;
		style.combo_arrow = tokens::col_text_dim;
		style.combo_hovered = tokens::col_card.alpha( 120 );
		style.combo_popup_bg = tokens::col_elevated.alpha( 170 );
		style.combo_popup_item_hovered = tokens::col_card.alpha( 125 );
		style.combo_popup_item_selected = tokens::col_accent.alpha( 42 );
		style.popup_bg = tokens::col_elevated.alpha( 170 );
		style.picker_bg = tokens::col_card;
		style.picker_popup_bg = tokens::col_elevated.alpha( 170 );
		style.text_input_bg = tokens::col_card;
		style.separator = tokens::col_accent.alpha( 30 );
		style.text = tokens::col_text;
		style.text_dim = tokens::col_text_dim;
		style.accent = tokens::col_accent;
		style.rounding = 9.0f;
		style.checkbox_rounding = 3.0f;
		style.slider_rounding = 2.0f;
		style.button_rounding = 6.0f;
		style.keybind_rounding = 2.0f;
		style.combo_rounding = 6.0f;
		style.popup_rounding = 7.0f;
		style.combo_popup_rounding = 6.0f;
		style.picker_popup_rounding = 7.0f;
		style.color_swatch_rounding = 2.0f;
		style.text_input_rounding = 6.0f;
		style.window_bg = tokens::col_elevated;
		style.window_border = xdraw::color{ 67, 63, 88, 255 };
		style.child_border = xdraw::color{ 59, 55, 75, 255 };
		style.border_thickness = 1.0f;
		style.text_input_h = 32.0f;
	}
	void menu::draw_top_bar( float w )
	{
		auto& dl = xui::draw::current( );
		const auto& input = xui::ctx( ).input;
		const auto content_x = this->m_x + tokens::gap + tokens::sidebar_w + tokens::gap;
		const auto bar_y = this->m_y + tokens::gap;
		if ( this->m_tab == static_cast< int >( tab::config ) )
		{
			dl.text( content_x, bar_y + 16.0f, "CONFIGS", tokens::col_text );
			dl.text( content_x, bar_y + 42.0f, "Save and load your Celestial setup", tokens::col_text_dim );
			const auto tabs_y = bar_y + 84.0f;
			constexpr auto tab_pad = 6.0f;
			constexpr auto tab_gap = 8.0f;
			const auto tab_w = ( w - tab_pad * 2.0f - tab_gap ) * 0.5f;
			dl.rect_filled( content_x, tabs_y, w, 50.0f, tokens::col_card, xdraw::corner_radius{ tokens::card_rounding } );
			dl.rect( content_x, tabs_y, w, 50.0f, xdraw::color{ 59, 55, 75, 255 }, xdraw::corner_radius{ tokens::card_rounding } );
			const auto my_tab = xui::rect{ content_x + tab_pad, tabs_y + 6.0f, tab_w, 38.0f };
			const auto workshop_tab = xui::rect{ my_tab.x + tab_w + tab_gap, tabs_y + 6.0f, tab_w, 38.0f };
			if ( input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
			{
				if ( input.in_rect( my_tab ) ) this->m_workshop_configs = false;
				if ( input.in_rect( workshop_tab ) ) this->m_workshop_configs = true;
			}
			for ( const auto& tab : { std::pair{ my_tab, "MY CONFIGS" }, std::pair{ workshop_tab, "WORKSHOP CONFIGS" } } )
			{
				const auto selected = ( tab.first.x == workshop_tab.x ) == this->m_workshop_configs;
				const auto hovered = input.in_rect( tab.first );
				dl.rect_filled( tab.first.x, tab.first.y, tab.first.w, tab.first.h,
					selected ? xdraw::color{ 49, 43, 75, 255 } : hovered ? xdraw::color{ 37, 35, 46, 255 } : tokens::col_elevated,
					xdraw::corner_radius{ tokens::btn_rounding } );
				dl.rect( tab.first.x, tab.first.y, tab.first.w, tab.first.h,
					selected ? tokens::col_accent : xdraw::color{ 83, 79, 99, 255 }, xdraw::corner_radius{ tokens::btn_rounding } );
				const auto [text_w, text_h] = xdraw::measure_text( tab.second );
				dl.text( tab.first.x + ( tab.first.w - text_w ) * 0.5f,
					tab.first.y + ( tab.first.h - text_h ) * 0.5f, tab.second,
					selected ? tokens::col_text : tokens::col_text_dim );
			}
			return;
		}
		const auto& def = k_subtab_defs[ this->m_tab ];
		const auto subtab_count = def.count;
		const auto inner_pad{ 4.0f };
		const auto subtab_h = tokens::subtab_bar_h - inner_pad * 2.0f;
		const auto subtabs_w = w;
		const auto btn_w = ( subtabs_w - inner_pad * 2.0f ) / static_cast< float >( subtab_count );
		dl.rect_filled( content_x, bar_y, subtabs_w, tokens::subtab_bar_h, tokens::col_card, xdraw::corner_radius{ tokens::card_rounding } );
		const auto by = bar_y + ( tokens::subtab_bar_h - subtab_h ) * 0.5f;
		for ( auto i = 0; i < subtab_count; ++i )
		{
			const auto bx = content_x + inner_pad + btn_w * i;
			const auto btn = xui::rect{ bx, by, btn_w, subtab_h };
			const auto hovered = input.in_rect( btn );
			if ( hovered && input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
			{
				this->m_subtab = i;
			}
			const auto is_active = ( this->m_subtab == i );
			const auto active_anim = xui::anim::lerp( xui::fnv1a( "subtab_text" ) + i, is_active ? 1.0f : 0.0f, 12.0f );
			const auto hover_anim = xui::anim::lerp( xui::fnv1a( "subtab_hover" ) + i, hovered && !is_active ? 1.0f : 0.0f, 14.0f );
			if ( is_active )
			{
				dl.rect_filled( btn.x, btn.y, btn.w, btn.h, xdraw::color{ 42, 38, 62, 255 }, xdraw::corner_radius{ tokens::btn_rounding } );
			}
			else if ( hover_anim > 0.01f )
			{
				dl.rect_filled( btn.x, btn.y, btn.w, btn.h, xdraw::color{ 37, 35, 46, 255 }, xdraw::corner_radius{ tokens::btn_rounding } );
			}
			dl.rect( btn.x, btn.y, btn.w, btn.h, is_active ? tokens::col_accent : xdraw::color{ 88, 85, 103, 255 }, xdraw::corner_radius{ tokens::btn_rounding } );
			const auto icon_name = k_subtab_icons[ this->m_tab ][ i ];
			const auto ico = icon_name != nullptr ? systems::g_icons.get( icon_name, k_subtab_icon_scale ) : nullptr;
			if ( ico && ico->texture && ico->width > 0 && ico->height > 0 )
			{
				// Same as ESP: draw at natural raster size, centred on the button.
				const auto iw = static_cast< float >( ico->width );
				const auto ih = static_cast< float >( ico->height );
				const auto ix = std::floor( btn.x + ( btn.w - iw ) * 0.5f );
				const auto iy = std::floor( btn.y + ( btn.h - ih ) * 0.5f );
				auto icon_col = xui::lerp( tokens::col_text_dim, tokens::col_accent, active_anim );
				icon_col = xui::lerp( icon_col, tokens::col_text, hover_anim * 0.35f );
				dl.image( ix, iy, iw, ih, ico->texture.Get( ), icon_col );
				continue;
			}
			const auto [tw, th] = xdraw::measure_text( def.names[ i ] );
			const auto tx = std::floor( btn.x + ( btn.w - tw ) * 0.5f );
			const auto ty = std::floor( btn.y + ( btn.h - th ) * 0.5f );
			auto text_col = xui::lerp( tokens::col_text_dim, tokens::col_text, active_anim );
			text_col = xui::lerp( text_col, tokens::col_text, hover_anim * 0.35f );
			dl.text( tx, ty, def.names[ i ], text_col );
		}
	}
}

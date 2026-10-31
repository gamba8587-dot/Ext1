// Complete drop-in replacement for:
// project/core/rendering/impl/menu/menu.core.cpp
// Fixes double window_bg overwrite (restores D3D11 glass blur), adds smooth lerped sidebar
// indicator, rounded sidebar container, gradient active states, and proper popup/combo borders.

#include <utilities/math/math.hpp>
#include <utilities/memory/memory.hpp>
#include <utilities/addresses/addresses.hpp>
#include <core/systems/systems.hpp>
#include <core/settings.hpp>
#include <core/resources/icons/rage.hpp>
#include <core/resources/icons/legit.hpp>
#include <core/resources/icons/visuals.hpp>
#include <core/resources/icons/world.hpp>
#include <core/resources/icons/skins.hpp>
#include <core/resources/icons/misc.hpp>
#include <core/resources/icons/configs.hpp>
#include "../../rendering.hpp"

namespace rendering {

namespace svgs {
constexpr auto star = R"(<svg width="32" height="32" viewBox="0 0 32 32" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M16 2L19.4 11.8L29.5 12.2L21.5 18.4L24.3 28.2L16 22.4L7.7 28.2L10.5 18.4L2.5 12.2L12.6 11.8L16 2Z" fill="#f43f5e"/></svg>)";
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
const auto dt = xdraw::delta_time( );
const auto target = this->m_open ? 1.0f : 0.0f;
const auto speed = this->m_open ? 22.0f : 28.0f;
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

xui::begin( );

const auto fade = xui::ease::out_cubic( (std::clamp)( this->m_open_anim, 0.0f, 1.0f ) );
if ( !this->m_open && fade <= 0.0f )
{
xui::end( );
return;
}

xui::draw::current( ).push_alpha( fade );
this->sync_theme_style( );

{
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

// Top window ambient accent glow line
dl.rect_filled_gradient(
wx + 16.0f, wy, ww - 32.0f, 1.5f,
tokens::col_accent.alpha( 0 ),
tokens::col_accent.alpha( 200 ),
tokens::col_accent.alpha( 200 ),
tokens::col_accent.alpha( 0 ) );

const auto sb_x = wx + tokens::gap;
const auto sb_y = wy + tokens::gap;
const auto sb_w = tokens::sidebar_w;
const auto sb_h = wh - tokens::gap * 2.0f;

// Rounded sidebar panel with subtle border
dl.rect_filled( sb_x, sb_y, sb_w, sb_h, tokens::col_dark.alpha( 235 ), xdraw::corner_radius{ tokens::card_rounding } );
dl.rect( sb_x, sb_y, sb_w, sb_h, xdraw::color{ 52, 48, 70, 180 }, xdraw::corner_radius{ tokens::card_rounding } );
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

// Brand header block with subtle glow
dl.rect_filled_gradient(
sb_x + 6.0f, sb_y + 6.0f, tokens::sidebar_w - 12.0f, 48.0f,
tokens::col_accent.alpha( 28 ), tokens::col_card.alpha( 10 ),
tokens::col_card.alpha( 10 ), tokens::col_accent.alpha( 12 ),
xdraw::corner_radius{ tokens::btn_rounding } );

if ( this->m_textures.logo.resource )
dl.image( sb_x + 10.0f, sb_y + 13.0f, 28.0f, 28.0f, this->m_textures.logo.resource.Get( ) );

dl.text( sb_x + 44.0f, sb_y + 13.0f, "CELESTIAL", tokens::col_text );
dl.text( sb_x + 44.0f, sb_y + 31.0f, "INTERNAL V2", tokens::col_accent );

dl.rect_filled_gradient(
sb_x + 10.0f, sb_y + 60.0f, tokens::sidebar_w - 20.0f, 1.0f,
tokens::col_accent.alpha( 85 ), xdraw::color{ 55, 52, 71, 40 },
xdraw::color{ 55, 52, 71, 40 }, tokens::col_accent.alpha( 85 ) );

const auto tabs_y = sb_y + 72.0f;
constexpr const char* labels[]{ "Ragebot", "Legitbot", "Visuals", "World", "Skins", "Misc", "Configs" };
constexpr auto tab_count{ 7 };

// Smoothly animated active tab indicator position
const auto target_indicator_y = tabs_y + static_cast<float>( this->m_tab ) * 48.0f;
const auto anim_indicator_y = xui::anim::lerp( xui::fnv1a( "sb_active_indicator" ), target_indicator_y, 18.0f, target_indicator_y );

for ( auto i = 0; i < tab_count; ++i )
{
const auto btn = xui::rect{ sb_x + 8.0f, tabs_y + i * 48.0f, tokens::sidebar_w - 16.0f, 42.0f };
const auto hovered = input.in_rect( btn );
const auto is_active = ( this->m_tab == i );

if ( hovered && input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
{
this->m_tab = i;
this->m_subtab = 0;
}

const auto active_t = xui::anim::lerp( xui::fnv1a( "sb_tab_act" ) + i, is_active ? 1.0f : 0.0f, 14.0f );
const auto hover_t = xui::anim::lerp( xui::fnv1a( "sb_tab_hov" ) + i, hovered && !is_active ? 1.0f : 0.0f, 16.0f );

if ( active_t > 0.01f || hover_t > 0.01f )
{
const auto bg_left = xui::lerp( tokens::col_card.alpha( 160 ), tokens::col_accent.alpha( 52 ), active_t );
const auto bg_right = xui::lerp( tokens::col_card.alpha( 90 ), tokens::col_card.alpha( 210 ), active_t );
dl.rect_filled_gradient(
btn.x, btn.y, btn.w, btn.h,
bg_left, bg_right, bg_right, bg_left,
xdraw::corner_radius{ tokens::btn_rounding } );

const auto border_col = xui::lerp( xdraw::color{ 62, 58, 82, 110 }, tokens::col_accent.alpha( 150 ), active_t );
dl.rect( btn.x, btn.y, btn.w, btn.h, border_col, xdraw::corner_radius{ tokens::btn_rounding } );
}

const auto icon_col = xui::lerp( tokens::col_text_dim.alpha( 185 ), tokens::col_accent, std::max( active_t, hover_t * 0.6f ) );
{
const auto& tex = this->m_textures.tabs[ i ];
const auto iw = static_cast< float >( tex.width );
const auto ih = static_cast< float >( tex.height );
const auto icon_x = btn.x + 10.0f;
const auto icon_y = std::floor( btn.y + ( btn.h - ih ) * 0.5f );
dl.image( icon_x, icon_y, iw, ih, tex.resource.Get( ), icon_col );
}

const auto text_col = xui::lerp( tokens::col_text_dim, tokens::col_text, std::max( active_t, hover_t * 0.7f ) );
dl.text( btn.x + 42.0f, btn.y + 13.0f, labels[ i ], text_col );
}

// Draw the smooth sliding vertical accent pill on the active tab
dl.rect_filled(
sb_x + 8.0f, anim_indicator_y + 8.0f, 3.5f, 26.0f,
tokens::col_accent, xdraw::corner_radius{ 2.0f } );

// Footer status card
const auto foot_y = sb_y + h - 46.0f;
dl.rect_filled( sb_x + 8.0f, foot_y, tokens::sidebar_w - 16.0f, 38.0f, tokens::col_card.alpha( 190 ), xdraw::corner_radius{ tokens::btn_rounding } );
dl.rect( sb_x + 8.0f, foot_y, tokens::sidebar_w - 16.0f, 38.0f, xdraw::color{ 58, 54, 76, 160 }, xdraw::corner_radius{ tokens::btn_rounding } );
dl.circle_filled( sb_x + 20.0f, foot_y + 13.0f, 3.5f, tokens::col_accent );
dl.text( sb_x + 29.0f, foot_y + 6.0f, "ACTIVE BUILD", tokens::col_text );
dl.text( sb_x + 20.0f, foot_y + 21.0f, "Celestial v2.4", tokens::col_text_dim );
}

void menu::sync_theme_style( ) const
{
auto& style = xui::ctx( ).style;
style.window_bg = tokens::col_elevated.alpha( 225 );
style.window_border = xdraw::color{ 74, 68, 98, 220 };
style.child_bg = tokens::col_card.alpha( 235 );
style.child_border = xdraw::color{ 62, 57, 82, 210 };
style.checkbox_bg = xdraw::color{ 28, 27, 38, 255 };
style.checkbox_border = xdraw::color{ 96, 92, 118, 255 };
style.checkbox_mark = tokens::col_accent;
style.checkbox_mark_icon = tokens::col_dark;
style.slider_track = xdraw::color{ 18, 17, 25, 255 };
style.slider_fill = tokens::col_accent;
style.button_bg = tokens::col_card;
style.button_border = xdraw::color{ 68, 63, 90, 200 };
style.button_hovered = xui::lighten( tokens::col_card, 1.22f );
style.button_active = tokens::col_accent;
style.keybind_bg = xdraw::color{ 22, 21, 31, 255 };
style.keybind_border = xdraw::color{ 72, 68, 94, 220 };
style.keybind_waiting = tokens::col_accent;
style.combo_bg = xdraw::color{ 21, 20, 30, 255 };
style.combo_border = xdraw::color{ 68, 63, 90, 220 };
style.combo_arrow = tokens::col_text_dim;
style.combo_hovered = xdraw::color{ 34, 32, 48, 255 };
style.combo_popup_bg = tokens::col_elevated.alpha( 245 );
style.combo_popup_border = xdraw::color{ 82, 76, 110, 240 };
style.combo_popup_item_hovered = tokens::col_card.alpha( 200 );
style.combo_popup_item_selected = tokens::col_accent.alpha( 55 );
style.popup_bg = tokens::col_elevated.alpha( 242 );
style.popup_border = xdraw::color{ 84, 78, 114, 235 };
style.picker_bg = tokens::col_card;
style.picker_border = xdraw::color{ 72, 68, 94, 220 };
style.picker_popup_bg = tokens::col_elevated.alpha( 245 );
style.picker_popup_border = xdraw::color{ 84, 78, 114, 235 };
style.text_input_bg = xdraw::color{ 20, 19, 28, 255 };
style.text_input_border = xdraw::color{ 70, 65, 92, 220 };
style.separator = tokens::col_accent.alpha( 38 );
style.text = tokens::col_text;
style.text_dim = tokens::col_text_dim;
style.accent = tokens::col_accent;

style.rounding = 7.0f;
style.checkbox_rounding = 4.0f;
style.slider_rounding = 3.0f;
style.button_rounding = 5.0f;
style.keybind_rounding = 4.0f;
style.combo_rounding = 6.0f;
style.popup_rounding = 8.0f;
style.combo_popup_rounding = 6.0f;
style.picker_popup_rounding = 8.0f;
style.color_swatch_rounding = 4.0f;
style.text_input_rounding = 6.0f;
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
dl.text( content_x, bar_y + 16.0f, "CONFIGURATIONS", tokens::col_text );
dl.text( content_x, bar_y + 40.0f, "Manage local & cloud profiles for Celestial v2", tokens::col_text_dim );

const auto tabs_y = bar_y + 84.0f;
constexpr auto tab_pad = 6.0f;
constexpr auto tab_gap = 8.0f;
const auto tab_w = ( w - tab_pad * 2.0f - tab_gap ) * 0.5f;

dl.rect_filled( content_x, tabs_y, w, 50.0f, tokens::col_card, xdraw::corner_radius{ tokens::card_rounding } );
dl.rect( content_x, tabs_y, w, 50.0f, xdraw::color{ 62, 57, 82, 220 }, xdraw::corner_radius{ tokens::card_rounding } );

const auto my_tab = xui::rect{ content_x + tab_pad, tabs_y + 6.0f, tab_w, 38.0f };
const auto workshop_tab = xui::rect{ my_tab.x + tab_w + tab_gap, tabs_y + 6.0f, tab_w, 38.0f };

if ( input.mouse_clicked && !xui::ctx( ).overlay_blocking( ) )
{
if ( input.in_rect( my_tab ) ) this->m_workshop_configs = false;
if ( input.in_rect( workshop_tab ) ) this->m_workshop_configs = true;
}

for ( const auto& tab : { std::pair{ my_tab, "LOCAL PROFILES" }, std::pair{ workshop_tab, "CLOUD WORKSHOP" } } )
{
const auto selected = ( tab.first.x == workshop_tab.x ) == this->m_workshop_configs;
const auto hovered = input.in_rect( tab.first );
dl.rect_filled( tab.first.x, tab.first.y, tab.first.w, tab.first.h,
selected ? tokens::col_accent.alpha( 45 ) : hovered ? xdraw::color{ 37, 35, 48, 255 } : tokens::col_elevated,
xdraw::corner_radius{ tokens::btn_rounding } );
dl.rect( tab.first.x, tab.first.y, tab.first.w, tab.first.h,
selected ? tokens::col_accent : xdraw::color{ 72, 68, 92, 200 }, xdraw::corner_radius{ tokens::btn_rounding } );

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
dl.rect( content_x, bar_y, subtabs_w, tokens::subtab_bar_h, xdraw::color{ 62, 57, 82, 210 }, xdraw::corner_radius{ tokens::card_rounding } );

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
const auto active_anim = xui::anim::lerp( xui::fnv1a( "subtab_text" ) + i, is_active ? 1.0f : 0.0f, 14.0f );
const auto hover_anim = xui::anim::lerp( xui::fnv1a( "subtab_hover" ) + i, hovered && !is_active ? 1.0f : 0.0f, 16.0f );

if ( active_anim > 0.01f || hover_anim > 0.01f )
{
const auto fill_col = xui::lerp( xdraw::color{ 35, 33, 46, 180 }, tokens::col_accent.alpha( 38 ), active_anim );
dl.rect_filled( btn.x + 2.0f, btn.y, btn.w - 4.0f, btn.h, fill_col, xdraw::corner_radius{ tokens::btn_rounding } );
}

if ( active_anim > 0.05f )
{
const auto pill_w = ( btn.w - 24.0f ) * active_anim;
dl.rect_filled( btn.x + ( btn.w - pill_w ) * 0.5f, btn.bottom( ) - 2.5f, pill_w, 2.5f, tokens::col_accent, xdraw::corner_radius{ 1.5f } );
}

const auto icon_name = k_subtab_icons[ this->m_tab ][ i ];
const auto ico = icon_name != nullptr ? systems::g_icons.get( icon_name, k_subtab_icon_scale ) : nullptr;
if ( ico && ico->texture && ico->width > 0 && ico->height > 0 )
{
const auto iw = static_cast< float >( ico->width );
const auto ih = static_cast< float >( ico->height );
const auto ix = std::floor( btn.x + ( btn.w - iw ) * 0.5f );
const auto iy = std::floor( btn.y + ( btn.h - ih ) * 0.5f );
auto icon_col = xui::lerp( tokens::col_text_dim, tokens::col_accent, active_anim );
icon_col = xui::lerp( icon_col, tokens::col_text, hover_anim * 0.4f );
dl.image( ix, iy, iw, ih, ico->texture.Get( ), icon_col );
continue;
}

const auto [tw, th] = xdraw::measure_text( def.names[ i ] );
const auto tx = std::floor( btn.x + ( btn.w - tw ) * 0.5f );
const auto ty = std::floor( btn.y + ( btn.h - th ) * 0.5f );
auto text_col = xui::lerp( tokens::col_text_dim, tokens::col_text, active_anim );
text_col = xui::lerp( text_col, tokens::col_text, hover_anim * 0.45f );
dl.text( tx, ty, def.names[ i ], text_col );
}
}
}

#pragma once
#include <string>
#include <vector>
#include <external/xdraw/xdraw.hpp>
namespace rendering {
	class context
	{
	public:
		bool initialize( IDXGISwapChain* swap_chain );
		void shutdown( );
		void on_present( IDXGISwapChain* swap_chain );
		void on_resize_buffers( );
		void on_resize_buffers_post( IDXGISwapChain* swap_chain );
		[[nodiscard]] HWND get_window( ) const { return this->m_window; }
		[[nodiscard]] ID3D11Device* get_device( ) const { return this->m_device; }
		[[nodiscard]] ID3D11DeviceContext* get_context( ) const { return this->m_context; }
		[[nodiscard]] bool is_initialized( ) const { return this->m_initialized; }
		[[nodiscard]] bool ui_assets_ready( ) const { return this->m_ui_assets_ready; }
	private:
		void create_rtv( IDXGISwapChain* swap_chain );
		void setup_zdraw( HWND window );
		bool try_bind_ui_assets( );
		ID3D11Device* m_device{ nullptr };
		ID3D11DeviceContext* m_context{ nullptr };
		ID3D11RenderTargetView* m_rtv{ nullptr };
		HWND m_window{ nullptr };
		bool m_initialized{ false };
		bool m_ui_assets_ready{ false };
	};
    class menu
    {
    public:
        void initialize_graphics( );
        void draw( );
        void shutdown( ) const;
        void toggle( ) { this->m_open = !this->m_open; }
		[[nodiscard]] bool is_open( ) const { return this->m_open; }
		void apply_saved_cursor( );
        enum class tab : int
        {
            ragebot, legitbot, player, world, skins, misc, config, count
        };
	private:
        void draw_side_bar( float h );
        void draw_top_bar( float w );
        void sync_theme_style( ) const;
        void draw_ragebot( float group_w ) const;
        void draw_legitbot( float group_w ) const;
        void draw_player( float group_w ) const;
        void draw_world( float group_w ) const;
        void draw_skins( float group_w ) const;
        void draw_misc( float group_w ) const;
        void draw_config( float group_w );
        bool m_open{ true };
        bool m_config_modal_open{};
        bool m_config_cloud_refresh_pending{};
        bool m_config_advanced_open{};
        bool m_workshop_configs{};
        bool m_last_open{ true };
        float m_open_anim{ 1.0f };
        std::uint8_t m_saved_relative_mouse{};
		bool m_has_saved_cursor{};
		int m_saved_cursor_x{};
		int m_saved_cursor_y{};
        float m_x{ 70.0f };
        float m_y{ 118.0f };
        float m_w{ 800.0f };
        float m_h{ 560.0f };
        bool m_initial_positioned{};
        float m_body_x{};
        float m_body_y{};
        float m_body_w{};
        float m_body_h{};
        int m_tab{ static_cast< int >( tab::config ) };
        int m_subtab{};
		bool m_graphics_ready{};
        struct textures
        {
            struct entry
            {
                Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource{};
                int width{};
                int height{};
            };
            entry logo{};
            entry tabs[ 7 ]{};
            entry cfg_folder_on{};
            entry cfg_folder_off{};
            entry cfg_cloud_on{};
            entry cfg_cloud_off{};
            entry cfg_plus{};
        } m_textures{};
        static constexpr auto k_max_subtabs{ 6 };
        // Equipment weapon SVGs are authored 32px tall (widths ~45..110). ESP draws them
// 1:1 at natural size, so match that: this keeps them pixel-aligned with the
// source artwork instead of resampling into a different box.
static constexpr float k_subtab_icon_scale{ 0.55f };
        struct subtab_info
        {
            const char* names[ k_max_subtabs ]{};
            int count{};
        };
        static constexpr subtab_info k_subtab_defs[ static_cast< int >( tab::count ) ]
        {
            { { "pistol", "smg", "rifle", "shotgun", "sniper", "lmg" }, 6 },
            { { "pistol", "smg", "rifle", "shotgun", "sniper", "lmg" }, 6 },
            { { "enemies", "allies", "local" },                         3 },
            { { "esp", "scene", "weather" },                            3 },
            { { "guns", "knives", "gloves", "agents" },                 4 },
            { { "main", "removals", "camera", "hud" },                  4 },
            { { "general" },                                            1 }
        };
        // Representative weapon icon per subtab slot; empty name falls back to text.
        static constexpr const char* k_subtab_icons[ static_cast< int >( tab::count ) ][ k_max_subtabs ]
        {
            { "glock", "p90", "ak47", "nova", "ssg08", "m249" },
            { "glock", "p90", "ak47", "nova", "ssg08", "m249" },
            { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
        };
    };
	class widgets
	{
	public:
		void draw( );
		static inline std::string s_map_name{};
	private:
		void watermark( xdraw::draw_list& draw_list );
		void keybinds( xdraw::draw_list& draw_list );
	};
	class fonts
	{
	public:
		void initialize( );
		xdraw::font* inter_bold{};
		xdraw::font* smallest_pixel7{};
	};
	inline context g_context{};
	inline menu g_menu{};
	inline widgets g_widgets{};
	inline fonts g_fonts{};
}

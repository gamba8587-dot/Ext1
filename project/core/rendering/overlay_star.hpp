#pragma once
#include <external/xdraw/xdraw.hpp>

namespace rendering {
	// Shared 20px mark for the watermark, keybind panel, and hit/miss logs.
	inline void draw_overlay_star( xdraw::draw_list& draw_list, float cx, float cy, xdraw::color color )
	{
		draw_list.line( cx, cy - 10.0f, cx, cy + 10.0f, color, 1.5f );
		draw_list.line( cx - 10.0f, cy, cx + 10.0f, cy, color, 1.5f );
		draw_list.line( cx - 7.0f, cy - 7.0f, cx + 7.0f, cy + 7.0f, color, 1.5f );
		draw_list.line( cx + 7.0f, cy - 7.0f, cx - 7.0f, cy + 7.0f, color, 1.5f );
	}
}

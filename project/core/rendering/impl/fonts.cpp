#include <core/resources/fonts/pixel7.hpp>
#include <core/resources/fonts/inter.hpp>
#include "../rendering.hpp"
namespace rendering {
	void fonts::initialize( )
	{
		this->inter_bold = xdraw::load_font( std::as_bytes( std::span{ resources::fonts::inter::bold } ), 17.0f );
		this->smallest_pixel7 = xdraw::load_font( std::as_bytes( std::span{ resources::fonts::pixel7::smallest } ), 10.5f );
	}
}

#include <utilities/memory/memory.hpp>
#include <signatures/game_addresses.hpp>
#include "../steam.hpp"
namespace steam {
	namespace detail {
		inline std::uintptr_t user_interface{};
	}
	bool user::initialize( )
	{
		detail::user_interface = memory::call<std::uintptr_t>( MODULE_EXPORT( "steam_api64.dll:SteamAPI_SteamUser_v023" ) );
		return detail::user_interface != 0;
	}
}

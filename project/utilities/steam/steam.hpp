#pragma once
namespace steam {
	class friends
	{
	public:
		static bool initialize( );
		static int get_medium_friend_avatar( std::uint64_t steam_id );
	};
	class user
	{
	public:
		static bool initialize( );
	};
	class utils
	{
	public:
		static bool initialize( );
		static bool get_image_size( int image, std::uint32_t* width, std::uint32_t* height );
		static bool get_image_rgba( int image, std::uint8_t* dest, int dest_size );
	};
}

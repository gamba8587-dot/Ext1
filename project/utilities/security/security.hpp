#pragma once
#define NOMINMAX
#include <Windows.h>
#include <external/nlohmann/json.hpp>
#include <vector>
#include <thread>
#include <atomic>
namespace security {
	namespace regions {
		struct region
		{
			std::uintptr_t base;
			std::size_t size;
		};
		void add( std::uintptr_t base, std::size_t size );
		void add_module( HMODULE module );
		bool contains( std::uintptr_t address, std::size_t size );
	}
	namespace prologues {
		struct hook_info
		{
			std::uintptr_t target;
			const std::uint8_t* bytes;
			std::size_t length;
		};
		void add( std::uintptr_t target, const std::uint8_t* bytes, std::size_t length );
		hook_info* get( std::uintptr_t address );
	}
}

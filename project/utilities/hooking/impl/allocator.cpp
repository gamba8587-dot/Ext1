#include <utilities/memory/memory.hpp>
#include <signatures/game_addresses.hpp>
#include "../hooking.hpp"
namespace hooking::allocator {
	namespace detail {
		static LONG nt_allocate_virtual_memory (HANDLE process, PVOID* base_addr, ULONG_PTR zero_bits, PSIZE_T region_size, ULONG type, ULONG protect) {
			return reinterpret_cast<LONG (__stdcall*)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG)>(MODULE_EXPORT ("ntdll.dll:NtAllocateVirtualMemory")) (process, base_addr, zero_bits, region_size, type, protect);
		}
		static LONG nt_free_virtual_memory (HANDLE process, PVOID* base_addr, PSIZE_T region_size, ULONG free_type) {
			return reinterpret_cast<LONG (__stdcall*)(HANDLE, PVOID*, PSIZE_T, ULONG)>(MODULE_EXPORT ("ntdll.dll:NtFreeVirtualMemory")) (process, base_addr, region_size, free_type);
		}
	}
	void* allocate (std::size_t size, void* near_) {
		const auto target = reinterpret_cast<std::uintptr_t>(near_);
		constexpr std::uintptr_t max_distance = 0x7fff0000;
		const auto min_addr = target > max_distance ? target - max_distance : 0x10000;
		const auto max_addr = target <= UINTPTR_MAX - max_distance ? target + max_distance : UINTPTR_MAX;
		const auto try_allocate = [ size, target ] ( std::uintptr_t requested ) -> void* {
			auto base = reinterpret_cast<void*>(requested);
			auto region = size;
			if ( detail::nt_allocate_virtual_memory ( GetCurrentProcess ( ), &base, 0, &region, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ) < 0 || !base ) {
				return nullptr;
			}
			const auto actual = reinterpret_cast<std::uintptr_t>(base);
			const auto distance = actual > target ? actual - target : target - actual;
			if ( distance <= max_distance ) {
				return base;
			}
			detail::nt_free_virtual_memory ( GetCurrentProcess ( ), &base, &region, MEM_RELEASE );
			return nullptr;
		};
		const auto center = target & ~0xffffull;
		constexpr std::uintptr_t granularity = 0x10000;
		for ( std::uintptr_t distance = granularity; distance <= max_distance; distance += granularity ) {
			if ( center >= min_addr + distance ) {
				if ( auto allocation = try_allocate ( center - distance ) ) return allocation;
			}
			if ( center <= max_addr - distance ) {
				if ( auto allocation = try_allocate ( center + distance ) ) return allocation;
			}
		}
		return nullptr;
	}
	void free (void* address) {
		if (!address) {
			return;
		}
		auto base = address;
		auto region {0ull};
		detail::nt_free_virtual_memory (GetCurrentProcess (), &base, &region, MEM_RELEASE);
	}
}

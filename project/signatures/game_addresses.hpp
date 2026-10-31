#ifndef GAME_ADDRESSES_HPP
#define GAME_ADDRESSES_HPP
#include <cstddef>
#include <cstdint>
#include <utilities/addresses/addresses.hpp>
#include <utilities/addresses/interfaces.hpp>
#include <utilities/memory/memory.hpp>
#include <signatures/patterns.hpp>
namespace signatures {
constexpr std::uint32_t hash_const (const char* str, std::uint32_t value = 0x811C9DC5u) {
	return *str ? hash_const (str + 1, (value ^ std::uint32_t (*str)) * 0x01000193u) : value;
}
template <std::size_t N>
consteval std::uint32_t hash (const char (&str) [N]) {
	return hash_const (str);
}
enum class resolve_op : std::uint8_t {
	direct,
	rel_call,
	rip_relative,
	absolute_ptr
};
struct signature_t {
	const char* module;
	const char* pattern;
	resolve_op op { resolve_op::direct };
	std::size_t op_byte_offset {};
	std::int64_t post_offset {};
	bool deref_final {};
};
}
#define SIGNATURE(module_name, pattern_string, ...) \
	(::signatures::signature_t{ module_name, pattern_string, ##__VA_ARGS__ })
#ifndef PATTERN
#define PATTERN(entry) \
	([]() -> std::uintptr_t { \
		static const auto val = memory::resolve_signature(entry); \
		return val; \
	}())
#endif
#ifndef INTERFACE_
#define INTERFACE_(str) \
	[]() -> std::uintptr_t { \
		static const auto val = memory::get_module_interface(str); \
		return val; \
	}()
#endif
#ifndef CONVAR
#define CONVAR(str) \
	[]() -> c_convar* { \
		static const auto val = addresses::globals::cvar->find(::signatures::hash(str)); \
		return val; \
	}()
#endif
#ifndef MODULE_BASE
#define MODULE_BASE(str) \
	[]() -> std::uintptr_t { \
		static const auto val = memory::get_module_base(str); \
		return val; \
	}()
#endif
#ifndef MODULE_EXPORT
#define MODULE_EXPORT(str) \
	([]() -> std::uintptr_t { \
		static std::uintptr_t val{}; \
		if (!val) val = memory::get_module_export(str); \
		return val; \
	}())
#endif
#endif

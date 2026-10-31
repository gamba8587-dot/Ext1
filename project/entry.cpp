#include <cstdio>
#include <crtdbg.h>
#include <windows.h>
extern "C" BOOL WINAPI _CRT_INIT(HMODULE hModule, DWORD dwReason, LPVOID lpReserved);
#include <utilities/logging/logging.hpp>
#include <utilities/addresses/addresses.hpp>
#include <utilities/security/security.hpp>
#include <utilities/memory/memory.hpp>
#include <utilities/threadpool/threadpool.hpp>
#include <utilities/steam/steam.hpp>
#include <core/hooks/hooks.hpp>
#include <core/systems/systems.hpp>
#include <core/settings.hpp>
#include <core/features/features.hpp>
#include <core/rendering/rendering.hpp>
#include <utilities/diag.hpp>
namespace {
	std::atomic<LPTOP_LEVEL_EXCEPTION_FILTER> g_previous_exception_filter{};
	PVOID g_vectored_exception_handler{};
	LONG WINAPI diag_unhandled_exception_filter( EXCEPTION_POINTERS* info );
	LONG CALLBACK diag_vectored_exception_filter( EXCEPTION_POINTERS* info )
	{
		if ( !info || !info->ExceptionRecord ||
			!diag::is_serious_exception( info->ExceptionRecord->ExceptionCode ) )
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		if ( diag::probe_active( ) )
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		const auto displaced_filter =
			SetUnhandledExceptionFilter( diag_unhandled_exception_filter );
		if ( displaced_filter != diag_unhandled_exception_filter )
		{
			g_previous_exception_filter.store(
				displaced_filter,
				std::memory_order_release );
		}
		if ( diag::is_module_address( info->ExceptionRecord->ExceptionAddress ) )
		{
			diag::record_crash(
				info,
				diag::g_exception_scope_depth
					? diag::g_exception_phase
					: "first-chance fault in velocity DLL" );
			return EXCEPTION_CONTINUE_SEARCH;
		}
		if ( diag::g_exception_scope_depth == 0 )
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		const auto code = info->ExceptionRecord->ExceptionCode;
		const auto instruction =
			reinterpret_cast<std::uintptr_t>( info->ExceptionRecord->ExceptionAddress );
		const auto accessed =
			info->ExceptionRecord->NumberParameters > 1
				? info->ExceptionRecord->ExceptionInformation[ 1 ]
				: 0;
		HMODULE fault_module{};
		char module_path[ MAX_PATH ]{ "unknown" };
		std::uintptr_t module_base{};
		if ( instruction &&
			GetModuleHandleExA(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
					GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				reinterpret_cast<LPCSTR>( instruction ), &fault_module ) )
		{
			module_base = reinterpret_cast<std::uintptr_t>( fault_module );
			GetModuleFileNameA( fault_module, module_path, MAX_PATH );
		}
		const auto* module_name = strrchr( module_path, '\\' );
		module_name = module_name ? module_name + 1 : module_path;
		char buf[ 384 ]{};
		_snprintf_s(
			buf, sizeof( buf ), _TRUNCATE,
			"FEATURE EXCEPTION [%s] 0x%08lX at %s+0x%llX (0x%p), accessed 0x%p",
			diag::g_exception_phase,
			code,
			module_name,
			module_base
				? static_cast<unsigned long long>( instruction - module_base )
				: 0ull,
			info->ExceptionRecord->ExceptionAddress,
			reinterpret_cast<void*>( accessed ) );
		diag::step( buf );
		return EXCEPTION_CONTINUE_SEARCH;
	}
	LONG WINAPI diag_unhandled_exception_filter( EXCEPTION_POINTERS* info )
	{
		if ( !info || !info->ExceptionRecord )
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		const auto instruction =
			reinterpret_cast<std::uintptr_t>( info->ExceptionRecord->ExceptionAddress );
		const auto accessed =
			info->ExceptionRecord->NumberParameters > 1
				? info->ExceptionRecord->ExceptionInformation[ 1 ]
				: 0;
		HMODULE fault_module{};
		char module_path[ MAX_PATH ]{ "unknown" };
		std::uintptr_t module_base{};
		if ( instruction &&
			GetModuleHandleExA(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
					GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				reinterpret_cast<LPCSTR>( instruction ), &fault_module ) )
		{
			module_base = reinterpret_cast<std::uintptr_t>( fault_module );
			GetModuleFileNameA( fault_module, module_path, MAX_PATH );
		}
		const auto* module_name = strrchr( module_path, '\\' );
		module_name = module_name ? module_name + 1 : module_path;
		char buf[ 384 ]{};
		_snprintf_s(
			buf, sizeof( buf ), _TRUNCATE,
			"UNHANDLED EXCEPTION 0x%08lX at %s+0x%llX (0x%p), accessed 0x%p",
			info->ExceptionRecord->ExceptionCode,
			module_name,
			module_base
				? static_cast<unsigned long long>( instruction - module_base )
				: 0ull,
			info->ExceptionRecord->ExceptionAddress,
			reinterpret_cast<void*>( accessed ) );
		diag::write( diag::level::fatal, buf );
		diag::record_crash( info, "unhandled exception" );
		const auto previous_filter =
			g_previous_exception_filter.load( std::memory_order_acquire );
		if ( previous_filter &&
			previous_filter != diag_unhandled_exception_filter )
		{
			return previous_filter( info );
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
#if defined( DEV )
	#define INIT_FAIL( msg ) \
		do { \
			diag::write( diag::level::error, msg ); \
			return 0; \
		} while ( 0 )
	#define INIT_WARN( msg ) diag::write( diag::level::warning, msg )
#else
	#define INIT_FAIL( msg ) \
		do { \
			diag::write( diag::level::error, msg ); \
			MessageBoxA( nullptr, xs( msg ), "...", MB_ICONERROR ); \
			return 0; \
		} while ( 0 )
	#define INIT_WARN( msg ) INIT_FAIL( msg )
#endif
	DWORD WINAPI init_thread_impl( LPVOID param )
	{
		const auto module_handle = static_cast<HMODULE>( param );
		diag::initialize_crash_dumps( );
		g_previous_exception_filter.store(
			SetUnhandledExceptionFilter( diag_unhandled_exception_filter ),
			std::memory_order_release );
		g_vectored_exception_handler =
			AddVectoredExceptionHandler( 1, diag_vectored_exception_filter );
		if ( !g_vectored_exception_handler )
		{
			diag::writef(
				diag::level::error,
				"failed to install vectored exception handler; win32_error=%lu",
				GetLastError( ) );
		}
		else
		{
			diag::write( diag::level::info, "crash handlers installed" );
		}
		const auto coinit_result =
			CoInitializeEx( nullptr, COINIT_MULTITHREADED );
		if ( FAILED( coinit_result ) )
		{
			diag::writef(
				diag::level::warning,
				"CoInitializeEx failed; hresult=0x%08lX",
				coinit_result );
		}
		config::initialize( );
		settings::finalize_binds( );
		security::regions::add_module( module_handle );
		{
			if ( !logging::console::initialize( ) )
			{
#if defined( DEV )
				INIT_WARN( "failed to initialize console logging." );
#else
				INIT_FAIL( "failed to initialize console logging." );
#endif
			}
			if ( !logging::popup::initialize( ) )
			{
#if defined( DEV )
				INIT_WARN( "failed to initialize popup logging." );
#else
				INIT_FAIL( "failed to initialize popup logging." );
#endif
			}
		}
		{
			if ( !threadpool::initialize( ) )
			{
				INIT_FAIL( "failed to initialize thread pool." );
			}
			if ( !steam::friends::initialize( ) )
			{
				INIT_FAIL( "failed to initialize steam friends." );
			}
			if ( !steam::user::initialize( ) )
			{
				INIT_FAIL( "failed to initialize steam user." );
			}
			if ( !steam::utils::initialize( ) )
			{
				INIT_FAIL( "failed to initialize steam utils." );
			}
		}
		{
			if ( !addresses::modules::initialize( ) )
			{
				INIT_FAIL( "failed to initialize module addresses." );
			}
			if ( !addresses::globals::initialize( ) )
			{
				INIT_FAIL( "failed to initialize global addresses." );
			}
			if ( !addresses::functions::initialize( ) )
			{
				INIT_FAIL( "failed to initialize function addresses." );
			}
		}
		{
			if ( !systems::materials::initialize( ) )
			{
				INIT_FAIL( "failed to initialize materials system." );
			}
			if ( !systems::events::initialize( ) )
			{
				INIT_FAIL( "failed to initialize event system." );
			}
			if ( !systems::g_icons.initialize( ) )
			{
				INIT_FAIL( "failed to initialize vpk parse system." );
			}
		}
		{
			if ( !features::changer::g_econ_item_system.initialize( ) )
			{
				INIT_FAIL( "failed to initialize econ item system." );
			}
		}
		{
			if ( !hooks::utility::initialize( ) )
			{
				INIT_FAIL( "failed to initialize utility hooks." );
			}
			if ( !hooks::cheat::initialize( ) )
			{
				INIT_FAIL( "failed to initialize cheat hooks." );
			}
		}
		{
			if ( !addresses::globals::cvar->unlock_all( ) )
			{
				INIT_FAIL( "failed to unlock hidden cvars." );
			}
		}
		features::world::g_scene.discover_skyboxes( );
		return 1;
	}
	DWORD diag_exception_filter( EXCEPTION_POINTERS* info )
	{
		char buf[ 128 ]{};
		_snprintf_s( buf, sizeof( buf ), _TRUNCATE, "EXCEPTION 0x%08lX at 0x%p", info->ExceptionRecord->ExceptionCode, info->ExceptionRecord->ExceptionAddress );
		diag::write( diag::level::fatal, buf );
		diag::record_crash( info, "initialization thread" );
		return EXCEPTION_EXECUTE_HANDLER;
	}
	DWORD WINAPI init_thread( LPVOID param )
	{
		__try
		{
			return init_thread_impl( param );
		}
		__except ( diag_exception_filter( GetExceptionInformation( ) ) )
		{
			return 0;
		}
	}
}
extern "C" int __stdcall entry( HMODULE module_handle, DWORD reason, LPVOID reserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		_CRT_INIT( module_handle, reason, reserved );
		DisableThreadLibraryCalls( module_handle );
		diag::set_module( module_handle );
		const auto thread = CreateThread( nullptr, 0, init_thread, module_handle, 0, nullptr );
		if ( !thread )
		{
			diag::writef(
				diag::level::error,
				"failed to create initialization thread; win32_error=%lu",
				GetLastError( ) );
			return 0;
		}
		CloseHandle( thread );
		return 1;
	}
	else if ( reason == DLL_PROCESS_DETACH )
	{
#if defined( DEV )
		if ( g_vectored_exception_handler )
		{
			RemoveVectoredExceptionHandler( g_vectored_exception_handler );
			g_vectored_exception_handler = nullptr;
		}
		const auto previous_filter =
			g_previous_exception_filter.exchange(
				nullptr,
				std::memory_order_acq_rel );
		const auto current_filter =
			SetUnhandledExceptionFilter( previous_filter );
		if ( current_filter != diag_unhandled_exception_filter )
		{
			SetUnhandledExceptionFilter( current_filter );
		}
		features::world::g_weather.release( );
		rendering::g_menu.shutdown( );
		systems::events::shutdown( );
		hooks::utility::shutdown( );
		hooks::cheat::shutdown( );
		CoUninitialize( );
#endif
		diag::shutdown( );
#if defined( DEV )
		_CRT_INIT( module_handle, reason, reserved );
#endif
	}
	return 1;
}

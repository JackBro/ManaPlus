/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define ENABLEDEBUGLOG 1

#define CAST_8 static_cast<char>
#define CAST_S8 static_cast<int8_t>
#define CAST_U8 static_cast<uint8_t>
#define CAST_S16 static_cast<int16_t>
#define CAST_U16 static_cast<uint16_t>
#define CAST_S32 static_cast<int32_t>
#define CAST_U32 static_cast<uint32_t>
#define CAST_SIZE static_cast<size_t>

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)

#ifdef __clang__
#define CLANG_VERSION (__clang_major__ * 10000 \
    + __clang_minor__ * 100 \
    + __clang_patchlevel__)
#endif  // __clang__

#if !defined(__GXX_EXPERIMENTAL_CXX0X__)
#undef nullptr
#define nullptr 0
#define final
#define override
#define constexpr
#define constexpr2
#define noexcept2
#define noexcept
#define A_DELETE(func)
#define A_DELETE_COPY(func)
#else  // !defined(__GXX_EXPERIMENTAL_CXX0X__)
#if GCC_VERSION < 40700

#if defined(__clang__) && __cplusplus > 201100L
#else  // __clang__
#define final
#define override
#define constexpr
#define noexcept
#endif  // __clang__
#define noexcept2
#define constexpr2

// #define A_DELETE
// #define A_DELETE_COPY
#else  // GCC_VERSION < 40700
#define ADVGCC
#if GCC_VERSION < 40900
#define constexpr2
#define noexcept2
#else  // GCC_VERSION < 40900
#if __cpp_constexpr >= 201304
#define constexpr2 constexpr
#else  // __cpp_constexpr >= 201304
#define constexpr2
#endif  // __cpp_constexpr >= 201304

#ifdef __clang__
#define noexcept2
#else  // __clang__
#define noexcept2 noexcept
#endif  // __clang__

#endif  // GCC_VERSION < 40900
#endif  // GCC_VERSION < 40700
#undef Z_NULL
#define Z_NULL nullptr
#define M_TCPOK
#define A_DELETE(func) func = delete
#define A_DELETE_COPY(name) name(const name &) = delete; \
    name &operator=(const name&) = delete;
#endif  // !defined(__GXX_EXPERIMENTAL_CXX0X__)

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#define A_WARN_UNUSED __attribute__ ((warn_unused_result))
#define DEPRECATED __attribute__ ((deprecated))
#ifdef DYECMD
#define A_DYECMD_UNUSED __attribute__ ((unused))
#else  // DYECMD
#define A_DYECMD_UNUSED
#endif  // DYECMD

#ifdef __native_client__
#define restrict
#define restrict2
#else  // __native_client__
#ifdef __clang__
// because restrict broken in clang, now it removed from all places.
// #define restrict __restrict__
#define restrict
#define restrict2
#else  // __clang__
#define restrict __restrict__
#define restrict2 __restrict__
#endif  // __clang__
#endif  // __native_client__

#ifndef ENABLE_CILKPLUS
#define A_CONST __attribute__ ((const))
#define A_PURE __attribute__ ((pure))
#define A_INLINE __attribute__ ((always_inline))
#else  // ENABLE_CILKPLUS
#define A_CONST
#define A_PURE
#define A_INLINE
#endif  // ENABLE_CILKPLUS

#ifdef __INTEL_COMPILER
#define RETURNS_NONNULL
#else  // __INTEL_COMPILER
#if GCC_VERSION < 40900
#define RETURNS_NONNULL
#else  // GCC_VERSION < 40900
#define RETURNS_NONNULL __attribute__((returns_nonnull))
#endif  // GCC_VERSION < 40900
#endif  // __INTEL_COMPILER

#ifndef ENABLE_CILKPLUS
#define A_NONNULL(...) __attribute__((nonnull (__VA_ARGS__)))
#else  // ENABLE_CILKPLUS
#define A_NONNULL(...)
#endif  // ENABLE_CILKPLUS

#else  // __GNUC__
#define A_UNUSED
#define A_WARN_UNUSED
#define gnu_printf printf
#define DEPRECATED
#define restrict
#define restrict2
#define RETURNS_NONNULL
#define A_NONNULL(...)
#endif  // __GNUC__
#ifdef __clang__
#define gnu_printf printf
#endif  // __clang__

#ifdef ENABLE_CILKPLUS
#include <cilk/cilk.h>
#endif  // ENABLE_CILKPLUS

#ifdef ADVGCC

#define const2 const
#if GCC_VERSION >= 60000
#define PRAGMA6(str) _Pragma(#str)
#else  // GCC_VERSION > 60000
#define PRAGMA6(str)
#endif  // GCC_VERSION > 60000
#if GCC_VERSION >= 70000
#define A_FALLTHROUGH __attribute__ ((fallthrough));
#else  // GCC_VERSION > 70000
#define A_FALLTHROUGH
#endif  // GCC_VERSION > 70000

#else  // ADVGCC
#define const2
#define PRAGMA6(str)
#define A_FALLTHROUGH
#endif  // ADVGCC

#ifdef __clang__
#define PRAGMACLANG(str) _Pragma(#str)
#if CLANG_VERSION >= 30800
#define PRAGMACLANG6(str) _Pragma(#str)
#define CLANG_FALLTHROUGH [[clang::fallthrough]];
#else  // __clang_major__ >= 6
#define PRAGMACLANG6(str)
#define CLANG_FALLTHROUGH
#endif  // __clang_major__ >= 6
#else  // __clang__
#define PRAGMACLANG(str)
#define PRAGMACLANG6(str)
#define CLANG_FALLTHROUGH
#endif  // __clang__

#ifdef __GNUC__
#if GCC_VERSION >= 40600
#define PRAGMA45(str) _Pragma(#str)
#elif defined(__clang__)
#define PRAGMA45(str) _Pragma(#str)
#else  // GCC_VERSION > 40600
#define PRAGMA45(str)
#endif  // GCC_VERSION > 40600
#endif  // __GNUC__

#ifdef __GNUC__
#ifdef ENABLE_CILKPLUS
#if GCC_VERSION < 40900
#define cilk_for for
#define cilk_spawn
#define cilk_sync
#elif GCC_VERSION < 50000
#ifdef cilk_for
#undef cilk_for
#endif  // cilk_for
#define cilk_for for
#endif  // GCC_VERSION < 40900
#else  // ENABLE_CILKPLUS
#define cilk_for for
#define cilk_spawn
#define cilk_sync
#endif  // ENABLE_CILKPLUS
#endif  // __GNUC__

#ifdef __GNUC__
#define A_LIKELY(x) __builtin_expect (!!(x), 1)
#define A_UNLIKELY(x) __builtin_expect (!!(x), 0)
#else  // __GNUC__
#define A_LIKELY(x) (x)
#define A_UNLIKELY(x) (x)
#endif  // __GNUC__

#define notfinal

#define FOR_EACH(type, iter, array) for (type iter = array.begin(), \
    iter##_fend = array.end(); iter != iter##_fend; ++ iter)

#define FOR_EACHR(type, iter, array) for (type iter = array.rbegin(), \
    iter##_fend = array.rend(); iter != iter##_fend; ++ iter)

#define FOR_EACHP(type, iter, array) for (type iter = array->begin(), \
    iter##_fend = array->end(); iter != iter##_fend; ++ iter)

#define FOR_EACH_SAFE(type, iter, array) for (type iter = array.begin(); \
    iter != array.end(); ++ iter)

#ifdef ENABLE_CHECKPLUGIN
#define A_NONNULLPOINTER __attribute__((nonnullpointer))
#else  // ENABLE_CHECKPLUGIN
#define A_NONNULLPOINTER
#endif  // ENABLE_CHECKPLUGIN

#ifdef ENABLE_CHECKS

#define CHECKLISTENERS \
    config.checkListeners(this, __FILE__, __LINE__); \
    serverConfig.checkListeners(this, __FILE__, __LINE__);

#else  // ENABLE_CHECKS

#define CHECKLISTENERS

#endif  // ENABLE_CHECKS

#ifdef USE_SDL2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#endif  // USE_SDL2
#include <SDL_version.h>
#ifdef USE_SDL2
#pragma GCC diagnostic pop
#endif  // USE_SDL2

#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef USE_SDL2
#warning using SDL2 headers but configure set to use SDL1.2
#warning please add configure flag --with-sdl2
#define USE_SDL2
#endif  // USE_SDL2
#else  // SDL_VERSION_ATLEAST(2, 0, 0)
#ifdef USE_SDL2
#error using SDL1.2 headers but configure set to use SDL2
#error please remove configure flag --with-sdl2
#endif  // USE_SDL2
#endif  // SDL_VERSION_ATLEAST(2, 0, 0)

// #define DEBUG_CONFIG 1
// #define DEBUG_BIND_TEXTURE 1
// #define DISABLE_RESOURCE_CACHING 1
#define DUMP_LEAKED_RESOURCES 1
// #define DEBUG_DUMP_LEAKS1 1
// #define DEBUG_SDLFONT 1

// android or nacl logging
// #define SPECIAL_LOGGING 1

// profiler
// #define USE_PROFILER 1

// draw calls
// #define DEBUG_DRAW_CALLS 1

// debug images usage
// #define DEBUG_IMAGES 1

// debug SDL surfaces
// #define DEBUG_SDL_SURFACES 1

// debug RWops usage
// #define DEBUG_PHYSFS 1

// use file access fuzzer
// #define USE_FUZZER 1

// use OpenGL debug features
// #define DEBUG_OPENGL 1

// debug animations
// #define DEBUG_ANIMATIONS 1

#ifdef DYECMD
#undef USE_FUZZER
#endif  // DYECMD
#include "utils/perfomance.h"
UTILS_PERFOMANCE_H  // guard for protect previous include
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

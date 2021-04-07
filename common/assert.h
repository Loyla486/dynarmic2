/* This file is part of the dynarmic project.
 * Copyright (c) 2020 MerryMage
 * SPDX-License-Identifier: 0BSD
 */

#pragma once

/*
#include <fmt/format.h>

#if defined(__clang__) || defined(__GNUC__)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define UNLIKELY(x) !!(x)
#endif

namespace Dynarmic::Common {

[[noreturn]] void Terminate(fmt::string_view msg, fmt::format_args args);

namespace detail {

template <typename... Ts>
[[noreturn]] void TerminateHelper(fmt::string_view msg, Ts... args) {
    Terminate(msg, fmt::make_format_args(args...));
}

} // namespace detail

} // namespace Dynarmic::Common

#if defined(NDEBUG)
    #if defined(__clang) || defined(__GNUC__)
        #define UNREACHABLE() __builtin_unreachable()
        #define ASSUME(expr) [&]{ if (!(expr)) __builtin_unreachable(); }()
    #elif defined(_MSC_VER)
        #define UNREACHABLE() __assume(0)
        #define ASSUME(expr) __assume(expr)
    #else
        #define UNREACHABLE() ASSERT_FALSE("Unreachable code!")
        #define ASSUME(expr)
    #endif
#else
    #define UNREACHABLE() ASSERT_FALSE("Unreachable code!")
    #define ASSUME(expr)
#endif

#ifdef DYNARMIC_IGNORE_ASSERTS
    #define ASSERT(expr) ASSUME(expr)
    #define ASSERT_MSG(expr, ...) ASSUME(expr)
    #define ASSERT_FALSE(...) UNREACHABLE()
#else
    #define ASSERT(expr)                                                                        \
        [&]{                                                                                    \
            if (UNLIKELY(!(expr))) {                                                            \
                ::Dynarmic::Common::detail::TerminateHelper(#expr);                             \
            }                                                                                   \
        }()
    #define ASSERT_MSG(expr, ...)                                                               \
        [&]{                                                                                    \
            if (UNLIKELY(!(expr))) {                                                            \
                ::Dynarmic::Common::detail::TerminateHelper(#expr "\nMessage: " __VA_ARGS__);   \
            }                                                                                   \
        }()
    #define ASSERT_FALSE(...) ::Dynarmic::Common::detail::TerminateHelper("false\nMessage: " __VA_ARGS__);
#endif

#if defined(NDEBUG) || defined(DYNARMIC_IGNORE_ASSERTS)
    #define DEBUG_ASSERT(expr) ASSUME(expr)
    #define DEBUG_ASSERT_MSG(expr, ...) ASSUME(expr)
#else
    #define DEBUG_ASSERT(expr) ASSERT(expr)
    #define DEBUG_ASSERT_MSG(expr, ...) ASSERT_MSG(expr, __VA_ARGS__)
#endif
*/

#include <cassert>
#include <iostream>

#define ASSERT(expr) assert(expr)
// if (UNLIKELY(!(expr)))
#define ASSERT_FALSE(...) std::cout << "ZOMG FALSE!" << std::endl << "Message: " <<  __VA_ARGS__; abort()
#define ASSERT_MSG(expr, ...) assert(expr)
#define DEBUG_ASSERT(expr) assert(expr)
#define UNREACHABLE() ASSERT_FALSE("Unreachable code!")

// OpenGL Object Wrapper
// Copyright 2016-2022 Sean Farrell <sean.farrell@rioki.org>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <vector>
#include <string>

// It's 2022, if your compiler does not have <format> in C++20
// mode, talk to your compiler vendor; I will not work around it.
#if !defined(NDEBUG) && defined(__cpp_lib_format)
#include <format>
#endif

#include "defines.h"

namespace glow
{
    //! Clear all pending OpenGL errors.
    GLOW_EXPORT void clear_errors() noexcept;

	//! Get the last OpenGL error as string.
    //!
    //!  @return the last OpenGL error as string
    GLOW_EXPORT const char* gl_error_to_string(unsigned int glerror) noexcept;

    //! Initialize glow in the current OpenGL context.
    //!
    //! You need to call init in each OpenGL context you intend to use glow
    //! before calling anyhting else.
    GLOW_EXPORT void init();

#if !defined(NDEBUG) && defined(__cpp_lib_format)
    constexpr std::string basename(const std::string& file) noexcept
    {
        auto i = file.find_last_of("\\/");
        if (i == std::string::npos)
        {
            return file;
        }
        else
        {
            return file.substr(i + 1);
        }
    }

    GLOW_EXPORT void trace(const std::string& file, const unsigned int line, const std::string_view msg) noexcept;

    template <typename Arg0, typename ... Args>
    void trace(const std::string& file, const unsigned int line, const std::string_view format, Arg0 arg0, Args ... args) noexcept
    {
        auto msg = std::format(format, arg0, std::forward<Args>(args)...);
        trace(file, line, msg);
    }

    GLOW_EXPORT void handle_assert(const std::string& file, const unsigned int line, const std::string_view scond) noexcept;

    GLOW_EXPORT void handle_fail(const std::string& file, const unsigned int line, const std::string_view message) noexcept;

    //! Check if there is an OpenGL error.
    GLOW_EXPORT void check_gl_error(const std::string& file, const unsigned int line) noexcept;
#endif

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}

#if !defined(NDEBUG) && defined(__cpp_lib_format)
#define GLOW_TRACE(MSG, ...)  ::glow::trace(::glow::basename(__FILE__), __LINE__, MSG, __VA_ARGS__)
#define GLOW_ASSERT(COND)    do { if (!(COND)) { ::glow::handle_assert(::glow::basename(__FILE__), __LINE__, #COND); } } while (false)
#define GLOW_FAIL(MSG)       ::glow::handle_fail(::glow::basename(__FILE__), __LINE__, MSG)
#define GLOW_CHECK_GLERROR() ::glow::check_gl_error(::glow::basename(__FILE__), __LINE__)
#else
#define GLOW_TRACE(MSG, ...)
#define GLOW_ASSERT(COND)
#define GLOW_FAIL(MSG)
#define GLOW_CHECK_GLERROR()
#endif

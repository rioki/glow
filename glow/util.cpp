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

#include "pch.h"
#include "util.h"

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <iostream>
#endif

namespace glow
{
    void clear_errors() noexcept
    {
        auto e = GLenum{};
        do
        {
            e = glGetError();
        }
        while (e != GL_NO_ERROR);
    }

    const char* gl_error_to_string(unsigned int glerror) noexcept
    {
        switch (glerror)
        {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            GLOW_FAIL("Unknown glerror type");
            return "UNKNOWN";
        }
    }

    void init()
    {
        static auto is_init  = false;
        if (is_init == false)
        {
            glewExperimental = GL_TRUE;
            auto err = glewInit();
            if (GLEW_OK != err)
            {
                throw std::runtime_error((const char*)glewGetErrorString(err));
            }

            clear_errors();
            is_init = true;
        }
    }

#if !defined(NDEBUG) && defined(__cpp_lib_format) && !defined(_WIN_UWP_)
    #ifdef _WIN32
    void show_message_box(const std::string_view& message) noexcept
    {
        auto r = MessageBoxA(NULL, message.data(), "Assert Failed", MB_ABORTRETRYIGNORE|MB_ICONSTOP);
        switch (r)
        {
        case IDIGNORE:
            // do nothing;
            break;
        case IDRETRY:
            _CrtDbgBreak();
            break;
        default:
            abort();
            break;
        }
    }
    #else
    // PORTME: Better suport for other platforms is desired, but not required.
    #endif

    void trace(const std::string& file, const unsigned int line, const std::string_view msg) noexcept
    {
        auto output = std::format("{}({}): {}\n", file, line, msg);
        #ifdef _WIN32
        OutputDebugStringA(output.data());
        #else
        std::cerr << output;
        #endif
    }

    void handle_assert(const std::string& file, const unsigned int line, const std::string_view scond) noexcept
    {
        auto msg = std::format("Assertion '{}' failed.", scond);
        trace(file, line, msg);
        #ifdef _WIN32
        show_message_box(msg);
        #else
        std::terminate();
        #endif
    }

    inline void handle_fail(const std::string& file, const unsigned int line, const std::string_view message) noexcept
    {
        auto msg = std::format("General Software Fault: '{}'.", message);
        trace(file, line, msg);
        #ifdef _WIN32
        show_message_box(msg);
        #else
        std::terminate();
        #endif
    }

    void check_gl_error(const std::string& file, const unsigned int line) noexcept
    {
        auto error = glGetError();
        if (error != GL_NO_ERROR)
        {
            auto msg = std::format("OpenGL Error: {}!", gl_error_to_string(error));
            trace(file, line, msg);
            #ifdef _WIN32
            show_message_box(msg);
            #else
            std::terminate();
            #endif
        }
    }
#endif
}

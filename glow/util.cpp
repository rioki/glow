//
// OpenGL Object Wrapper
// 
// Copyright (c) 2016-2019 Sean Farrell <sean.farrell@rioki.org>
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
// 

#include "util.h"

#include <cassert>
#include <stdexcept>
#include <atomic>
#include <GL/glew.h>

namespace glow
{
    void clear_errors()
    {
        GLenum e;
        do
        {
            e = glGetError();
        }
        while (e != GL_NO_ERROR);
    }

    std::string get_error()
    {
        std::string result;
        
        GLenum e = glGetError();
        switch (e)
        {
            case GL_NO_ERROR:
                result = "GL_NO_ERROR";
                break;
            case GL_INVALID_ENUM:
                result = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                result = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                result = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                result = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                result = "GL_OUT_OF_MEMORY";
                break;
            default:
                assert(false && "invalud id");
                break;
        }

        return result;
    }
    
    // NOTE: atomic should not be necessary, since glow works on only one thread anyway.
    // But keep it in here, because it's good practice.
    std::atomic<bool> is_init(false);

    void init()
    {   
        bool i = is_init.exchange(true);
        if (i == false)
        {
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (GLEW_OK != err)
            {
                throw std::runtime_error((const char*)glewGetErrorString(err));
            }

            clear_errors();
        }
    }
}
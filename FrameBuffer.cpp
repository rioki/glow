//
// OpenGL Object Wrapper
// 
// Copyright (c) 2016 Sean Farrell <sean.farrell@rioki.org>
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

#include "FrameBuffer.h"

#include <cassert>
#include <gl/glew.h>

#include "util.h"

namespace glow
{
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, 
                         GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2,
                         GL_COLOR_ATTACHMENT3,
                         GL_COLOR_ATTACHMENT4,
                         GL_COLOR_ATTACHMENT5,
                         GL_COLOR_ATTACHMENT6,
                         GL_COLOR_ATTACHMENT7,
                         GL_COLOR_ATTACHMENT8,
                         GL_COLOR_ATTACHMENT9,
                         GL_COLOR_ATTACHMENT10,
                         GL_COLOR_ATTACHMENT11,
                         GL_COLOR_ATTACHMENT12,
                         GL_COLOR_ATTACHMENT13,
                         GL_COLOR_ATTACHMENT14,
                         GL_COLOR_ATTACHMENT15};
    GLenum front[] = { GL_FRONT };


    FrameBuffer::FrameBuffer()
    : bound(false), glid(0) 
    {
        glGenFramebuffers(1, &glid);    

        assert(glGetError() == GL_NO_ERROR); 
    }


    FrameBuffer::~FrameBuffer()
    {
        if (glid != 0)
        {
            glDeleteFramebuffers(1, &glid);

            assert(glGetError() == GL_NO_ERROR); 
        }
    }

    void FrameBuffer::bind()
    {
        assert(glid != 0);

        glBindFramebuffer(GL_FRAMEBUFFER, glid);
        bound = true;

        assert(glGetError() == GL_NO_ERROR); 
    }

    void FrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        bound = false;
        
        assert(glGetError() == GL_NO_ERROR); 
    }

    void FrameBuffer::attach_depth(Texture& texture)
    {
        assert(glid != 0);
        assert(bound);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.glid, 0);
        
        assert(glGetError() == GL_NO_ERROR); 
    }

    void FrameBuffer::attach(unsigned int slot, Texture& texture, unsigned int level)
    {        
        assert(glid != 0);
        assert(bound);
        assert(slot < 15);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, texture.glid, level);
        glNamedFramebufferDrawBuffers(glid, slot + 1, buffers);

        assert(glGetError() == GL_NO_ERROR); 
    }

    void FrameBuffer::attach(unsigned int slot, Texture& texture, CubeFace face, unsigned int level)
    {
        assert(glid != 0);
        assert(bound);
        assert(slot < 15);

        GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, target, texture.glid, level);

        glNamedFramebufferDrawBuffers(glid, slot + 1, buffers);

        assert(glGetError() == GL_NO_ERROR); 
    }
}
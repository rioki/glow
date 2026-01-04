// OpenGL Object Wrapper
// Copyright 2016-2026 Sean Farrell <sean.farrell@rioki.org>
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

#include "defines.h"

#include "Texture.h"

namespace glow
{
    enum class CubeFace;

    //! Frame Buffer Wrapper
    class GLOW_EXPORT FrameBuffer
    {
    public:
        //! Create empty frame buffer.
        FrameBuffer();

        //! Destroy frame buffer.
        ~FrameBuffer();

        //! Bind the frame buffer for writing.
        void bind();

        //! Unbind the frame buffer.
        void unbind();

        //! Attach depth buffer.
        void attach_depth(Texture& texture);

        //! Attach color buffer.
        //!
        //! @param slot the slot to use
        //! @param texture the texture to write to
        //! @param level the mipmap level to write to
        void attach(unsigned int slot, Texture& texture, unsigned int level = 0);

        //! Attach cubamap face as color buffer.
        //!
        //! @param slot the slot to use
        //! @param texture the texture to write to
        //! @param face the cubemap face
        //! @param level the mipmap level to write to
        void attach(unsigned int slot, Texture& texture, CubeFace face, unsigned int level = 0);

    private:
        bool         bound;
        unsigned int glid;

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator = (const FrameBuffer&) = delete;

    };
}

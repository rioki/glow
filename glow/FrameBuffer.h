//
// OpenGL Object Wrapper
//
// Copyright 2016-2019 Sean Farrell <sean.farrell@rioki.org>
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

#ifndef _GLOW_FRAME_BUFFER_H_
#define _GLOW_FRAME_BUFFER_H_

#include "defines.h"

#include "Texture.h"

namespace glow
{
    enum class CubeFace;

    class GLOW_EXPORT FrameBuffer
    {
    public:

        FrameBuffer();

        ~FrameBuffer();

        void bind();

        void unbind();

        void attach_depth(Texture& texture);

        void attach(unsigned int slot, Texture& texture, unsigned int level = 0);

        void attach(unsigned int slot, Texture& texture, CubeFace face, unsigned int level = 0);

    private:
        bool         bound;
        unsigned int glid;
    };
}

#endif

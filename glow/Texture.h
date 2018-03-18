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

#ifndef _GLOW_TEXTURE_H_
#define _GLOW_TEXTURE_H_

#include "defines.h"

namespace glow
{
    enum ColorMode
    {
        NO_COLOR_MODE,
        RGB,
        RGBA,
        DEPTH
    };

    enum TextureType
    {
        NO_TEXTURE,
        TEXTURE2D,
        CUBE_MAP
    };

    enum CubeFace
    {
        XPOS,
        XNEG,
        YPOS,
        YNEG,
        ZPOS,
        ZNEG
    };

    enum FilterMode
    {
        FILTER_NEAREST,
        FILTER_LINEAR
    };

    class GLOW_EXPORT Texture
    {
    public:
        
        Texture();

        Texture(const Texture&) = delete;

        ~Texture();

        const Texture& operator = (const Texture&) = delete;

        TextureType get_type() const;

        unsigned int get_width() const;

        unsigned int get_height() const;

        ColorMode get_color_mode() const;

        void bind(unsigned int slot);

        void unbind(unsigned int slot);

        void upload_2d(unsigned int width, unsigned int height, ColorMode mode, void* data = nullptr, FilterMode filter = FILTER_LINEAR);

        void upload_2d_float(unsigned int width, unsigned int height, ColorMode mode, float* data = nullptr, FilterMode filter = FILTER_LINEAR);

        void upload_cube(unsigned int resolution, ColorMode mode, void* xpos = nullptr, void* xneg = nullptr, void* ypos = nullptr, void* yneg = nullptr, void* zpos = nullptr, void* zneg = nullptr, FilterMode filter = FILTER_LINEAR);

        void generate_mipmaps();

        int get_mipmap_levels() const;

    private:
        TextureType  type;
        unsigned int glid;
        unsigned int width;
        unsigned int height;
        ColorMode mode;

    friend class FrameBuffer;
    };
}

#endif

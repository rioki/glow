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

#ifndef _GLOW_TEXTURE_H_
#define _GLOW_TEXTURE_H_

#include <glm/glm.hpp>

#include "defines.h"

namespace glow
{
    /*!
     * Color Mode
     */
    enum class ColorMode
    {
        NO_COLOR_MODE,
        MONO,
        RGB,
        RGBA,
        DEPTH
    };

    /*!
     * Texture Type
     */
    enum class TextureType
    {
        NO_TEXTURE,
        TEXTURE2D,
        CUBE_MAP
    };

    /*!
     * Cube map face.
     */
    enum class CubeFace
    {
        XPOS,
        XNEG,
        YPOS,
        YNEG,
        ZPOS,
        ZNEG
    };

    /*!
     * Filter Mode
     */
    enum class FilterMode
    {
        FILTER_NEAREST,
        FILTER_LINEAR
    };

    /*!
     * Texture
     * 
     * The textrue class can represent GL_TEXTURE_2D and GL_TEXTURE_CUBE_MAP.
     */
    class GLOW_EXPORT Texture
    {
    public:
        /*!
         * Create an ampry texture handle.
         */
        Texture();

        /*!
         * Release texture handle.
         */
        ~Texture();

        /*!
         * Get texture type.
         *
         * @return the type of texture
         */
        TextureType get_type() const;

        /*!
         * Get texture size.
         *
         * @return the size of texture in pixels
         */
        glm::uvec2 get_size() const;

        /*!
         * Get texture color mode.
         *
         * @return the color mode of texture
         */
        ColorMode get_color_mode() const;

        /*!
         * Bind texture to a given slot.
         *
         * @param slot the texture slot to bind the texture to.
         *
         * @see Shader::set_uniform
         */
        void bind(unsigned int slot);

        /*!
         * Unbind texture.
         *
         * @param slot the texture slot to bind the texture to.
         */
        void unbind(unsigned int slot);

        /*!
         * Upload a 2D unsinged byte texture.
         * 
         * @param size the size in pixels
         * @param mode the color mode
         * @param data the memory to upload
         * @param filter the filter mode           
         */
        void upload_2d(glm::uvec2 size, ColorMode mode, void* data = nullptr, FilterMode filter = FilterMode::FILTER_LINEAR);

        /*!
         * Upload a 2D float texture.
         * 
         * @param size the size in pixels
         * @param mode the color mode
         * @param data the memory to upload
         * @param filter the filter mode           
         */
        void upload_2d_float(glm::uvec2 size, ColorMode mode, float* data = nullptr, FilterMode filter = FilterMode::FILTER_LINEAR);

        /*!
         * Upload a cube map texture.
         * 
         * @param size the size in pixels
         * @param mode the color mode
         * @param xpos the memory to upload
         * @param xneg the memory to upload
         * @param ypos the memory to upload
         * @param yneg the memory to upload
         * @param zpos the memory to upload
         * @param zneg the memory to upload
         * @param filter the filter mode           
         */
        void upload_cube(glm::uint resolution, ColorMode mode, void* xpos = nullptr, void* xneg = nullptr, void* ypos = nullptr, void* yneg = nullptr, void* zpos = nullptr, void* zneg = nullptr, FilterMode filter = FilterMode::FILTER_LINEAR);

        /*!
         * Ask OpenGL to generate mipmaps for this texture.
         */
        void generate_mipmaps();

        /*!
         * Get the total mipmap levels for this texture.
         */
        int get_mipmap_levels() const;

    private:
        TextureType  type = TextureType::NO_TEXTURE;
        glm::uint    glid = 0u;
        glm::uvec2   size = {0u, 0u};
        ColorMode    mode = ColorMode::NO_COLOR_MODE;

        Texture(const Texture&) = delete;
        const Texture& operator = (const Texture&) = delete;


    friend class FrameBuffer;
    };
}

#endif

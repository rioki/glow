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

#include <string_view>
#include <glm/glm.hpp>

#include "defines.h"

namespace glow
{
    //! Color Mode
    enum class ColorMode
    {
        UNKNOWN,
        R,
        RG,
        RGB,
        BGR,
        RGBA,
        BGRA,
        DEPTH
    };

    //! Data Type
    enum class DataType
    {
        UNKNOWN,
        INT8,
        UINT8,
        INT16,
        UINT16,
        UINT32,
        INT32,
        FLOAT,
        DOUBLE
    };

    //! Texture Type
    enum class TextureType
    {
        NO_TEXTURE,
        TEXTURE2D,
        CUBE_MAP
    };

    //! Cube map face.
    enum class CubeFace
    {
        XPOS,
        XNEG,
        YPOS,
        YNEG,
        ZPOS,
        ZNEG
    };

    //! Filter Mode
    enum class FilterMode
    {
        NEAREST,
        LINEAR
    };

    //! Wrap Mode
    enum class WrapMode
    {
        CLAMP,
        REPEAT
    };

    //! Texture
    //!
    //! The textrue class can represent GL_TEXTURE_2D and GL_TEXTURE_CUBE_MAP.
    class GLOW_EXPORT Texture
    {
    public:
        //! Create an ampry texture handle.
        //!
        //! @param debug_label The label to use for debug purposes.
        Texture(const std::string_view debug_label = "unnamed") noexcept;

        //! Release texture handle.
        ~Texture();

        //! Get texture type.
        //!
        //! @return the type of texture
        TextureType get_type() const noexcept;

        //! Get texture size.
        //!
        //! @return the size of texture in pixels
        glm::uvec2 get_size() const noexcept;

        //! Get texture color mode.
        //!
        //! @return the color mode of texture
        ColorMode get_color_mode() const noexcept;

        //! Get texture data type mode.
        //!
        //! @return the data type of texture
        DataType get_data_type() const noexcept;

        //! Bind texture to a given slot.
        //!
        //! @param slot the texture slot to bind the texture to.
        //!
        //! @see Shader::set_uniform
        void bind(glm::uint slot) noexcept;

        //! Unbind texture.
        void unbind() noexcept;

        //! Upload a 2D unsinged byte texture.
        //!
        //! @param size the size in pixels
        //! @param color the color mode
        //! @param type the data type
        //! @param data the memory to upload
        //! @param filter the filter mode
        //! @param wrap the wrtapping mode
        void upload_2d(glm::uvec2 size, ColorMode color, DataType data, const void* bits = nullptr, FilterMode filter = FilterMode::LINEAR, WrapMode wrap = WrapMode::REPEAT) noexcept;

        //! Upload a cube map texture.
        //!
        //! @param size the size in pixels
        //! @param mode the color mode
        //! @param xpos the memory to upload
        //! @param xneg the memory to upload
        //! @param ypos the memory to upload
        //! @param yneg the memory to upload
        //! @param zpos the memory to upload
        //! @param zneg the memory to upload
        //! @param filter the filter mode
        void upload_cube(glm::uint resolution, ColorMode mode, DataType data, const void* xpos = nullptr, const void* xneg = nullptr, const void* ypos = nullptr, const void* yneg = nullptr, const void* zpos = nullptr, const void* zneg = nullptr, FilterMode filter = FilterMode::LINEAR) noexcept;

        //! Ask OpenGL to generate mipmaps for this texture.
        void generate_mipmaps() noexcept;

        //! Get the total mipmap levels for this texture.
        int get_mipmap_levels() const noexcept;

    private:
        TextureType  type  = TextureType::NO_TEXTURE;
        glm::uint    glid  = 0u;
        glm::uvec2   size  = {0u, 0u};
        ColorMode    color = ColorMode::UNKNOWN;
        DataType     data  = DataType::UNKNOWN;

        glm::uint    last_slot = 0;

        Texture(const Texture&) = delete;
        const Texture& operator = (const Texture&) = delete;

    friend class FrameBuffer;
    };
}

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

#include "pch.h"
#include "Texture.h"
#include "util.h"

namespace glow
{
    Texture::Texture(const std::string_view debug_label) noexcept
    {
        glGenTextures(1, &glid);
        GLOW_CHECK_GLERROR();

        #ifndef NDEBUG
        glBindTexture(GL_TEXTURE_2D, glid);
        glObjectLabel(GL_TEXTURE, glid, static_cast<GLsizei>(debug_label.size()), debug_label.data());
        GLOW_CHECK_GLERROR();
        glBindTexture(GL_TEXTURE_2D, 0);
        #endif
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &glid);
        glid = 0;
        GLOW_CHECK_GLERROR();
    }

    TextureType Texture::get_type() const noexcept
    {
        return type;
    }

    glm::uvec2 Texture::get_size() const noexcept
    {
        return size;
    }

    ColorMode Texture::get_color_mode() const noexcept
    {
        return color;
    }

    DataType Texture::get_data_type() const noexcept
    {
        return data;
    }

    void Texture::bind(glm::uint slot) noexcept
    {
        GLOW_ASSERT(glid != 0);

        switch (type)
        {
            case TextureType::NO_TEXTURE:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                break;
            case TextureType::TEXTURE2D:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, glid);
                break;
            case TextureType::CUBE_MAP:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_CUBE_MAP, glid);
                break;
            default:
                GLOW_FAIL("invalid type");
                break;
        }

        last_slot = slot;
        GLOW_CHECK_GLERROR();
    }

    void Texture::unbind() noexcept
    {
        glActiveTexture(GL_TEXTURE0 + last_slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLenum glinternalformat(ColorMode color, DataType data) noexcept
    {
        switch (data)
        {
        case DataType::INT8:
            switch (color)
            {
            case ColorMode::R:
                return GL_R8I;
            case ColorMode::RG:
                return GL_RG8I;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB8I;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA8I;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA8I;
            }
        case DataType::UINT8:
            switch (color)
            {
            case ColorMode::R:
                return GL_RED;
            case ColorMode::RG:
                return GL_RG;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA;
            }
        case DataType::INT16:
            switch (color)
            {
            case ColorMode::R:
                return GL_R16I;
            case ColorMode::RG:
                return GL_RG16I;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB16I;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA16I;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA16I;
            }
        case DataType::UINT16:
            switch (color)
            {
            case ColorMode::R:
                return GL_R16UI;
            case ColorMode::RG:
                return GL_RG16UI;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB16UI;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA16UI;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA16UI;
            }
        case DataType::INT32:
            switch (color)
            {
            case ColorMode::R:
                return GL_R32I;
            case ColorMode::RG:
                return GL_RG32I;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB32I;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA32I;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA32I;
            }
        case DataType::UINT32:
            switch (color)
            {
            case ColorMode::R:
                return GL_R32UI;
            case ColorMode::RG:
                return GL_RG32UI;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB32UI;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA32UI;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA32UI;
            }
        case DataType::FLOAT:
        case DataType::DOUBLE:
            switch (color)
            {
            case ColorMode::R:
                return GL_R32F;
            case ColorMode::RG:
                return GL_RG32F;
            case ColorMode::RGB:
            case ColorMode::BGR:
                return GL_RGB32F;
            case ColorMode::RGBA:
            case ColorMode::BGRA:
                return GL_RGBA32F;
            default:
                GLOW_FAIL("Unexpected color mode.");
                return GL_RGBA32F;
            }
        default:
            GLOW_FAIL("Unexpected type.");
            return GL_RGB;
        }
    }

    GLenum glformat(ColorMode color) noexcept
    {
        switch (color)
        {
        case ColorMode::R:
            return GL_RED;
        case ColorMode::RG:
            return GL_RG;
        case ColorMode::RGB:
            return GL_RGB;
        case ColorMode::BGR:
            return GL_BGR;
        case ColorMode::RGBA:
            return GL_RGBA;
        case ColorMode::BGRA:
            return GL_BGRA;
        default:
            GLOW_FAIL("Unexpected color mode.");
            return GL_RGB;
        }
    }

    GLenum gltype(DataType data) noexcept
    {
        switch (data)
        {
        case DataType::INT8:
            return GL_BYTE;
        case DataType::UINT8:
            return GL_UNSIGNED_BYTE;
        case DataType::INT16:
            return GL_SHORT;
        case DataType::UINT16:
            return GL_UNSIGNED_SHORT;
        case DataType::INT32:
            return GL_INT;
        case DataType::UINT32:
            return GL_UNSIGNED_INT;
        case DataType::FLOAT:
            return GL_FLOAT;
        case DataType::DOUBLE:
            return GL_DOUBLE;
        default:
            GLOW_FAIL("Unexpected type.");
            return GL_RGB;
        }
    }

    void Texture::upload_2d(glm::uvec2 s, ColorMode c, DataType d, const void* memory, FilterMode filter, WrapMode wrap) noexcept
    {
        GLOW_ASSERT(glid != 0);
        GLOW_ASSERT(c != ColorMode::UNKNOWN);
        GLOW_ASSERT(d != DataType::UNKNOWN);

        type  = TextureType::TEXTURE2D;
        size  = s;
        color = c;
        data  = d;

        glBindTexture(GL_TEXTURE_2D, glid);

        switch (filter)
        {
        case FilterMode::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case FilterMode::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        default:
            GLOW_FAIL("Unknown filter mode.");
            break;
        }

        switch (wrap)
        {
        case WrapMode::CLAMP:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case WrapMode::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        default:
            GLOW_FAIL("Unknown wrap mode.");
            break;
        }

        GLenum gl_internalformat = glinternalformat(color, data);
        GLenum gl_format         = glformat(color);
        GLenum gl_type           = gltype(data);
        glTexImage2D(GL_TEXTURE_2D, 0, gl_internalformat, size.x, size.y, 0, gl_format, gl_type, memory);

        glBindTexture(GL_TEXTURE_2D, 0);
        GLOW_CHECK_GLERROR();
    }

    void Texture::upload_cube(unsigned int resolution, ColorMode c, DataType d, const void* xpos, const void* xneg, const void* ypos, const void* yneg, const void* zpos, const void* zneg, FilterMode filter) noexcept
    {
        GLOW_ASSERT(glid != 0);

        type  = TextureType::CUBE_MAP;
        size  = glm::uvec2(resolution);
        color = c;
        data  = d;

        GLenum gl_internalformat = glinternalformat(color, data);
        GLenum gl_format         = glformat(color);
        GLenum gl_type           = gltype(data);

        glBindTexture(GL_TEXTURE_CUBE_MAP, glid);

        if (filter == FilterMode::NEAREST)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, xpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, xneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, ypos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, yneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, zpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, gl_internalformat, resolution, resolution, 0, gl_format, gl_type, zneg);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        GLOW_CHECK_GLERROR();
    }

    void Texture::generate_mipmaps() noexcept
    {
        switch (type)
        {
            case TextureType::TEXTURE2D:
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, glid);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                break;
            case TextureType::CUBE_MAP:
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, glid);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                break;
            default:
                assert(false && "no texture to mipmap");
                break;
        }

        // NOTE: make this configurable
        auto aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }

    int Texture::get_mipmap_levels() const noexcept
    {
        int result = 0;
        switch (type)
        {
            case TextureType::TEXTURE2D:
                glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &result);
                break;
            case TextureType::CUBE_MAP:
                glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, &result);
                break;
            default:
                GLOW_FAIL("no texture to mipmap");
                break;
        }
        return result;
    }
}

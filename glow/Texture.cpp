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

#include "Texture.h"

#include <cassert>
#include <GL/glew.h>

#include "Shader.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"

namespace glow
{
    GLenum color2gl(ColorMode mode)
    {
        switch (mode)
        {
        case ColorMode::MONO:
            return GL_ALPHA;
        case ColorMode::RGB:
            return GL_RGB;
        case ColorMode::RGBA:
            return GL_RGBA;
        case ColorMode::DEPTH:
            return GL_DEPTH_COMPONENT;
        default:
            assert(false && "invalid mode");
            return 0;
        }
    }

    GLenum nchanels(ColorMode mode)
    {
        switch (mode)
        {
        case ColorMode::MONO:
            return 1;
        case ColorMode::RGB:
            return 3;
        case ColorMode::RGBA:
            return 4;
        case ColorMode::DEPTH:
            return 1;
        default:
            assert(false && "invalid mode");
            return 0;
        }
    }

    GLenum color2gl_float(ColorMode mode)
    {
        switch (mode)
        {
        case ColorMode::MONO:
            return GL_ALPHA32F_EXT;
        case ColorMode::RGB:
            return GL_RGB32F;
        case ColorMode::RGBA:
            return GL_RGBA32F;
        case ColorMode::DEPTH:
            return GL_DEPTH_COMPONENT32F;
        default:
            assert(false && "invalid mode");
            return 0;
        }
    }

    Texture::Texture()
    {
        glGenTextures(1, &glid);
        assert(glGetError() == GL_NO_ERROR);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &glid);
        glid = 0;
        assert(glGetError() == GL_NO_ERROR);
    }

    TextureType Texture::get_type() const
    {
        return type;
    }

    glm::uvec2 Texture::get_size() const
    {
        return size;
    }

    ColorMode Texture::get_color_mode() const
    {
        return mode;
    }

    void Texture::bind(unsigned int slot)
    {
        assert(glid != 0);

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
                assert(false && "invalid type");
                break;
        }


        assert(glGetError() == GL_NO_ERROR);
    }

    // REVIEW why not remeber the slot this texture was bound to?
    void Texture::unbind(unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::upload_2d(glm::uvec2 s, ColorMode m, void* data, FilterMode filter)
    {
        assert(glid != 0);

        type = TextureType::TEXTURE2D;
        size = s;
        mode = m;
        mode = m;

        glBindTexture(GL_TEXTURE_2D, glid);

        // TODO mipmap
        if (filter == FilterMode::FILTER_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        GLenum glmode = color2gl(mode);

        glTexImage2D(GL_TEXTURE_2D, 0, glmode, size[0], size[1], 0, glmode, GL_UNSIGNED_BYTE, data);


        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glGetError() == GL_NO_ERROR);
    }

    // REVIEW consider rolling upload_2d and upload_2d_float into one function.
    void Texture::upload_2d_float(glm::uvec2 s, ColorMode m, float* data, FilterMode filter)
    {
        assert(glid != 0);

        type = TextureType::TEXTURE2D;
        size = s;
        mode = m;

        glBindTexture(GL_TEXTURE_2D, glid);

        // TODO mipmap
        if (filter == FilterMode::FILTER_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        GLenum glintmode = color2gl_float(mode);
        GLenum glmode = color2gl(mode);
        glTexImage2D(GL_TEXTURE_2D, 0, glintmode, size[0], size[1], 0, glmode, GL_FLOAT, data);


        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glGetError() == GL_NO_ERROR);
    }

    void Texture::upload_cube(unsigned int resolution, ColorMode m, void* xpos, void* xneg, void* ypos, void* yneg, void* zpos, void* zneg, FilterMode filter)
    {
        assert(glid != 0);

        type = TextureType::CUBE_MAP;
        size = glm::uvec2(resolution);
        mode = m;

        GLenum glmode = color2gl(mode);

        glBindTexture(GL_TEXTURE_CUBE_MAP, glid);

        if (filter == FilterMode::FILTER_NEAREST)
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

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, xpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, xneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, ypos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, yneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, zpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, glmode, resolution, resolution, 0, glmode, GL_UNSIGNED_BYTE, zneg);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        assert(glGetError() == GL_NO_ERROR);
    }

    void Texture::generate_mipmaps()
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
        float aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }

    int Texture::get_mipmap_levels() const
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
                assert(false && "no texture to mipmap");
                break;
        }
        return result;
    }
}
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
        case RGB:
            return GL_RGB;
        case RGBA:
            return GL_RGBA;
        case DEPTH:
            return GL_DEPTH_COMPONENT;
        default:
            assert(false && "invalid mode");
            return 0;
        }
    }

    GLenum color2gl_float(ColorMode mode)
    {
        switch (mode)
        {
        case RGB:
            return GL_RGB32F;
        case RGBA:
            return GL_RGBA32F;        
        case DEPTH:
            return GL_DEPTH_COMPONENT32F;
        default:
            assert(false && "invalid mode");
            return 0;
        }
    }

    

    Texture::Texture()
    : type(NO_TEXTURE), glid(0), width(0), height(0), mode(NO_COLOR_MODE)
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

    unsigned int Texture::get_width() const
    {
        return width;
    }


    unsigned int Texture::get_height() const
    {
        return height;
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
            case NO_TEXTURE:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                break;
            case TEXTURE2D:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, glid);
                break;
            case CUBE_MAP:
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_CUBE_MAP, glid);
                break;
            default:
                assert(false && "invalid type");
                break;
        }
        

        assert(glGetError() == GL_NO_ERROR);
    }

    void Texture::unbind(unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::upload_2d(unsigned int w, unsigned int h, ColorMode m, void* data, FilterMode filter)
    {
        assert(glid != 0);

        type   = TEXTURE2D;
        width  = w;
        height = h;
        mode   = m;

        glBindTexture(GL_TEXTURE_2D, glid);

        // TODO mipmap
        if (filter == FILTER_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        }

        if (m == DEPTH)
        {
            /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);*/
        }
        
        GLenum glmode = color2gl(mode);
        glTexImage2D(GL_TEXTURE_2D, 0, glmode, width, height, 0, glmode, GL_UNSIGNED_BYTE, data);

        
        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glGetError() == GL_NO_ERROR);
    }

    void Texture::upload_2d_float(unsigned int w, unsigned int h, ColorMode m, float* data, FilterMode filter)
    {
        assert(glid != 0);

        type   = TEXTURE2D;
        width  = w;
        height = h;
        mode   = m;

        glBindTexture(GL_TEXTURE_2D, glid);

        // TODO mipmap
        if (filter == FILTER_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        }

        if (m == DEPTH)
        {
            /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);*/
        }

        GLenum glintmode = color2gl_float(mode);
        GLenum glmode = color2gl(mode);
        glTexImage2D(GL_TEXTURE_2D, 0, glintmode, width, height, 0, glmode, GL_FLOAT, data);


        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glGetError() == GL_NO_ERROR);
    }

    void Texture::upload_cube(unsigned int resolution, ColorMode m, void* xpos, void* xneg, void* ypos, void* yneg, void* zpos, void* zneg, FilterMode filter)
    {
        assert(glid != 0);

        type   = CUBE_MAP;
        width  = resolution;
        height = resolution;
        mode   = m;

        GLenum glmode = color2gl(mode);

        glBindTexture(GL_TEXTURE_CUBE_MAP, glid);

        if (filter == FILTER_NEAREST)
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
            case TEXTURE2D:
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, glid);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                break;
            case CUBE_MAP:
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
            case TEXTURE2D:
                glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &result);
                break;
            case CUBE_MAP:
                glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, &result);
                break;
            default:
                assert(false && "no texture to mipmap");
                break;
        }
        return result;
    }
}
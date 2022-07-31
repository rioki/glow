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

#include "pch.h"
#include "VertexBuffer.h"
#include "util.h"

namespace glow
{
    VertexBuffer::VertexBuffer() noexcept
    {
        glGenVertexArrays(1, &vao);
        GLOW_CHECK_GLERROR();
    }

    VertexBuffer::~VertexBuffer()
    {
        for (auto& info: buffers)
        {
            if (info.glid != 0)
            {
                glDeleteBuffers(1, &info.glid);
                info.glid = 0;
            }
        }

        for (auto& info: indexes)
        {
            if (info.glid != 0)
            {
                glDeleteBuffers(1, &info.glid);
                info.glid = 0;
            }
        }

        glDeleteVertexArrays(1, &vao);
        vao = 0;

        GLOW_CHECK_GLERROR();
    }

    void VertexBuffer::bind(Shader& shader) noexcept
    {
        glBindVertexArray(vao);

        for (auto& info: buffers)
        {
            info.adr = shader.get_attribute(info.attribute);
            if (info.adr != -1)
            {
                glBindBuffer(GL_ARRAY_BUFFER, info.glid);
                glVertexAttribPointer(info.adr, info.stride, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(info.adr);
                GLOW_CHECK_GLERROR();
            }
        }

        GLOW_CHECK_GLERROR();
    }

    void VertexBuffer::unbind() noexcept
    {
        glBindVertexArray(0);
        GLOW_CHECK_GLERROR();
    }

    void VertexBuffer::upload_values(const std::string& attribute, unsigned int stride, unsigned int count, const float* data) noexcept
    {
        unsigned int glid;
        glGenBuffers(1, &glid);
        glBindBuffer(GL_ARRAY_BUFFER, glid);
        glBufferData(GL_ARRAY_BUFFER, count * stride * sizeof(float), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        buffers.push_back({attribute, stride, count, glid, 0});

        GLOW_CHECK_GLERROR();
    }

    void VertexBuffer::upload_indexes(FacesType type, unsigned int count, const unsigned int* data) noexcept
    {
        auto glid = 0u;
        glGenBuffers(1, &glid);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glid);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        indexes.push_back({type, count, glid});

        GLOW_CHECK_GLERROR();
    }

    constexpr GLenum get_gl_facetype(FacesType type)
    {
        switch (type)
        {
        case FacesType::POINTS:
            return GL_POINTS;
        case FacesType::LINES:
            return GL_LINES;
        case FacesType::LINE_STRIP:
            return GL_LINE_STRIP;
        case FacesType::LINE_LOOP:
            return GL_LINE_LOOP;
        case FacesType::TRIANGLES:
            return GL_TRIANGLES;
        case FacesType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case FacesType::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        default:
            GLOW_FAIL("Unknown FaceType");
            return GL_TRIANGLES;
        }
    }

    void VertexBuffer::draw(unsigned int set) noexcept
    {
        GLOW_ASSERT(set < indexes.size());

        auto iinfo    = indexes[set];
        auto gl_shape = get_gl_facetype(iinfo.type);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iinfo.glid);
        glDrawElements(gl_shape, iinfo.count, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLOW_CHECK_GLERROR();
    }
}

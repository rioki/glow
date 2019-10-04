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

#include "VertexBuffer.h"

#include <cassert>
#include <GL/glew.h>

namespace glow
{
    VertexBuffer::VertexBuffer()
    : bound(false), vao(0) 
    {
        glGenVertexArrays(1, &vao);

        assert(glGetError() == GL_NO_ERROR); 
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

        assert(glGetError() == GL_NO_ERROR); 
    }

    void VertexBuffer::bind(Shader& shader)
    {
        glBindVertexArray(vao);
        bound = true;

        for (auto& info: buffers)
        {
            info.adr = shader.get_attribute(info.attribute);
            if (info.adr != -1)
            {
                glBindBuffer(GL_ARRAY_BUFFER, info.glid);
                glVertexAttribPointer(info.adr, info.stride, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(info.adr);
                assert(glGetError() == GL_NO_ERROR);
            }
        }

        assert(glGetError() == GL_NO_ERROR); 
    }

    void VertexBuffer::unbind()
    {
        glBindVertexArray(0);
        bound = false;

        assert(glGetError() == GL_NO_ERROR); 
    }

    void VertexBuffer::upload_values(const std::string attribute, unsigned int stride, unsigned int count, const float* data)
    {
        unsigned int glid;
        glGenBuffers(1, &glid);
        glBindBuffer(GL_ARRAY_BUFFER, glid);
        glBufferData(GL_ARRAY_BUFFER, count * stride * sizeof(float), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
         
        BufferInfo info = {attribute, stride, count, glid, 0};
        buffers.push_back(info);

        assert(glGetError() == GL_NO_ERROR); 
    }

    void VertexBuffer::upload_indexes(FacesType type, unsigned int count, const unsigned int* data)
    {
        unsigned int glid;
        glGenBuffers(1, &glid);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glid);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        IndexInfo info = {type, count, glid};
        indexes.push_back(info);

        assert(glGetError() == GL_NO_ERROR); 
    }

    void VertexBuffer::draw(unsigned int set)
    {
        assert(bound);
        assert(set < indexes.size());        

        
        IndexInfo iinfo = indexes[set];

        GLenum gl_shape;
        switch (iinfo.type)
        {
            case POINTS:
                gl_shape = GL_POINTS;
                break;
            case LINES:
                gl_shape = GL_LINES;
                break;
            case LINE_STRIP:
                gl_shape = GL_LINE_STRIP;
                break;
            case LINE_LOOP:
                gl_shape = GL_LINE_LOOP;
                break;
            case TRIANGLES:
                gl_shape = GL_TRIANGLES;
                break;
            case TRIANGLE_STRIP:
                gl_shape = GL_TRIANGLE_STRIP;
                break;
            case TRIANGLE_FAN:
                gl_shape = GL_TRIANGLE_FAN;
                break;
            default:
                assert(false && "invalid shape");
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iinfo.glid);
        glDrawElements(gl_shape, iinfo.count, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLenum error = glGetError();
        assert(error == GL_NO_ERROR);
        //assert(glGetError() == GL_NO_ERROR);
    }
}

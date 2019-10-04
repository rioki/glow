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

#ifndef _GLOW_VERTEX_BUFFER_H_
#define _GLOW_VERTEX_BUFFER_H_

#include "defines.h"

#include <string>
#include <vector>

#include "Shader.h"

namespace glow
{
    enum FacesType
    {
        POINTS,
        LINES,
        LINE_STRIP,
        LINE_LOOP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN        
    };

    class GLOW_EXPORT VertexBuffer
    {
    public:
        
        VertexBuffer();

        ~VertexBuffer();

        void bind(Shader& shader);

        void unbind();

        void upload_values(const std::string attribute, unsigned int stride, unsigned int count, const float* data);

        void upload_indexes(FacesType type, unsigned int count, const unsigned int* data);

        void draw(unsigned int set = 0);

    private:
        struct BufferInfo
        {
            std::string  attribute;
            unsigned int stride;
            unsigned int count;
            unsigned int glid;
            unsigned int adr;
        };
        
        struct IndexInfo
        {
            FacesType     type;
            unsigned int  count;
            unsigned int  glid;
        };

        bool                    bound;
        unsigned int            vao;
        std::vector<BufferInfo> buffers;
        std::vector<IndexInfo>  indexes;
    };
}

#endif

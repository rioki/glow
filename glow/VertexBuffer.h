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

#ifndef _GLOW_VERTEX_BUFFER_H_
#define _GLOW_VERTEX_BUFFER_H_

#include "defines.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace glow
{
    /*!
     * Fact Type
     */
    enum class FacesType
    {
        POINTS,
        LINES,
        LINE_STRIP,
        LINE_LOOP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN
    };

    /*!
     * Vertex Buffer
     */
    class GLOW_EXPORT VertexBuffer
    {
    public:
        /*! 
         * Create and empty vertex buffer.
         */
        VertexBuffer();

        /*! 
         * Free vertex buffer.
         */
        ~VertexBuffer();

        /*!
         * Bind vertex buffer to shader.
         *
         * @param shader to bind to
         */
        void bind(Shader& shader);

        /*!
         * Unbind vertex buffer.
         */
        void unbind();

        /*!
         * Upload vertex values.
         */
        void upload_values(const std::string& attribute, unsigned int stride, unsigned int count, const float* data);

        /*!
         * Upload vertex values.
         */
        template <int N, glm::qualifier Q>
        void upload_values(const std::string& attribute, const std::vector<glm::vec<N, float, Q>>& values);

        /*!
         * Upload face indexes.
         */
        void upload_indexes(FacesType type, unsigned int count, const unsigned int* data);

        /*!
         * Upload face indexes.
         */
        template <int N, glm::qualifier Q>
        void upload_indexes(const std::vector<glm::vec<N, glm::uint, Q>>& indexes);

        /*!
         * Draw vertex buffer.
         */
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

    template <int N, glm::qualifier Q>
    void VertexBuffer::upload_values(const std::string& attribute, const std::vector<glm::vec<N, float, Q>>& values)
    {
        upload_values(attribute, N, values.size(), glm::value_ptr(values[0]));
    }

    template <int N, glm::qualifier Q>
    void VertexBuffer::upload_indexes(const std::vector<glm::vec<N, glm::uint, Q>>& indexes)
    {
        FacesType type;
        switch (N)
        {
            case 1:
                type = FacesType::POINTS;
                break;
            case 2:
                type = FacesType::LINES;
                break;
            case 3:
                type = FacesType::TRIANGLES;
                break;
            case 4:
                throw std::logic_error("OpenGL does not support vertex buffers with quads.");
            default:
                throw std::logic_error("Unknown index type.");
        }
        upload_indexes(type, indexes.size() * N, glm::value_ptr(indexes[0]));
    }
}

#endif

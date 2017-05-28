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

#ifndef _GLOW_SHADER_H_
#define _GLOW_SHADER_H_

#include <vector>
#include <string>

#include "defines.h"

namespace glow
{
    class GLOW_EXPORT Shader 
    {
    public:
        Shader();

        ~Shader();

        void set_vertex_code(const std::string& value);

        const std::string& get_vertex_code() const;

        void set_fragment_code(const std::string& value);

        const std::string& get_fragment_code() const;

        void compile();

        void bind();

        void unbind();

        void set_uniformi(const std::string& name, int value);
        void set_uniformf(const std::string& name, float value);
        void set_uniform2i(const std::string& name, int x, int y);
        void set_uniform2f(const std::string& name, float x, float y);        
        void set_uniform3i(const std::string& name, int x, int y, int z);
        void set_uniform3f(const std::string& name, float x, float y, float z);        
        void set_uniform4i(const std::string& name, int x, int y, int z, int m);
        void set_uniform4f(const std::string& name, float x, float y, float z, float m);        
        void set_uniform_matrix2(const std::string& name, const float* value);
        void set_uniform_matrix3(const std::string& name, const float* value);
        void set_uniform_matrix4(const std::string& name, const float* value);
        void set_uniformiv(const std::string& name, const std::vector<int>& value); 
        void set_uniformfv(const std::string& name, const std::vector<float>& value); 

        unsigned int get_attribute(const std::string& name);

        void bind_output(const std::string& name, unsigned int channel);

    private:
        bool         bound;
        std::string  vertex_code;
        std::string  fragment_code;
        unsigned int program_id;
    };
}

#endif

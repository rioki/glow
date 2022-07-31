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
#include "Shader.h"

#include "util.h"

using namespace std::string_view_literals;

namespace glow
{
    constexpr auto CODE_PREFIX =
        "#version 430\n"
        "#define GLOW_VERSION 20\n"; // 0.2.0

    constexpr auto VERTEX_PREFIX =
        "#define GLOW_VERTEX\n";

    constexpr auto FRAGMENT_PREFIX =
        "#define GLOW_FRAGMENT\n";

    Shader::Shader(const std::string& c)
    : code(c)
    {
        compile();
    }

    Shader::~Shader()
    {
        if (program_id != 0)
        {
            glDeleteProgram(program_id);
            program_id = 0;
            GLOW_CHECK_GLERROR();
        }
    }

    void Shader::set_code(const std::string& value) noexcept
    {
        code = value;
    }

    const std::string& Shader::get_code() const noexcept
    {
        return code;
    }

    void Shader::compile()
    {
        GLOW_ASSERT(program_id == 0);

        auto status = 0;
        auto logstr = std::array<char, 256>();

        auto vbuff = std::array<const char*, 3>{CODE_PREFIX, VERTEX_PREFIX, code.data()};

        unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_id, static_cast<GLsizei>(vbuff.size()), vbuff.data(), NULL);
        glCompileShader(vertex_id);

        glGetShaderInfoLog(vertex_id, static_cast<GLsizei>(logstr.size()), NULL, logstr.data());

        glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            throw std::runtime_error(logstr.data());
        }

        GLOW_CHECK_GLERROR();

        auto fbuff = std::array<const char*, 3>{CODE_PREFIX, FRAGMENT_PREFIX, code.data()};

        unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_id,  static_cast<GLsizei>(fbuff.size()), fbuff.data(), NULL);
        glCompileShader(fragment_id);

        glGetShaderInfoLog(fragment_id, static_cast<GLsizei>(logstr.size()), NULL, logstr.data());

        glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);
            throw std::runtime_error(logstr.data());
        }

        GLOW_CHECK_GLERROR();

        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_id);
        assert(glGetError() == GL_NO_ERROR);
        glAttachShader(program_id, fragment_id);
        assert(glGetError() == GL_NO_ERROR);
        glLinkProgram(program_id);

        glGetShaderInfoLog(vertex_id,  static_cast<GLsizei>(logstr.size()), NULL, logstr.data());

        glGetProgramiv(program_id, GL_LINK_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);
            glDeleteProgram(program_id);
            throw std::runtime_error(logstr.data());
        }

        // NOTE: glDeleteShader() actually does not delete the shader, it only
        // flags the shader for deletion. The shaders will be deleted when
        // the program gets deleted.
        glDeleteShader(vertex_id);
        glDeleteShader(fragment_id);

        GLOW_CHECK_GLERROR();
    }

    void Shader::bind() noexcept
    {
        GLOW_ASSERT(program_id != 0);
        glUseProgram(program_id);
        GLOW_CHECK_GLERROR();
    }

    void Shader::unbind() noexcept
    {
        glUseProgram(0);
        GLOW_CHECK_GLERROR();
    }

    void Shader::set_uniform(const std::string_view id, const UniformValue& value) noexcept
    {
        GLOW_ASSERT(program_id != 0);
        if (int location = glGetUniformLocation(program_id, id.data()); location != -1)
        {
            std::visit(overloaded {
            [&] (bool v)         { glUniform1i(location, v); },
            [&] (int v)          { glUniform1i(location, v); },
            [&] (uint v)         { glUniform1i(location, v); },
            [&] (float v)        { glUniform1f(location, v); },
            [&] (const ivec2& v) { glUniform2i(location, v.x, v.y); },
            [&] (const uvec2& v) { glUniform2i(location, v.x, v.y); },
            [&] (const vec2& v)  { glUniform2f(location, v.x, v.y); },
            [&] (const ivec3& v) { glUniform3i(location, v.x, v.y, v.z); },
            [&] (const uvec3& v) { glUniform3i(location, v.x, v.y, v.z); },
            [&] (const vec3& v)  { glUniform3f(location, v.x, v.y, v.z); },
            [&] (const ivec4& v) { glUniform4i(location, v.x, v.y, v.z, v.w); },
            [&] (const uvec4& v) { glUniform4i(location, v.x, v.y, v.z, v.w); },
            [&] (const vec4& v)  { glUniform4f(location, v.x, v.y, v.z, v.w); },
            [&] (const mat2& v)  { glUniformMatrix2fv(location, 1u, GL_FALSE, glm::value_ptr(v)); },
            [&] (const mat3& v)  { glUniformMatrix3fv(location, 1u, GL_FALSE, glm::value_ptr(v)); },
            [&] (const mat4& v)  { glUniformMatrix4fv(location, 1u, GL_FALSE, glm::value_ptr(v)); }
            }, value);
        }
    }

    void Shader::set_uniform(const std::string_view name, Texture& texture) noexcept
    {
        auto slot = get_texture_slot(name);
        texture.bind(slot);
        set_uniform(name, slot);
    }

    uint Shader::get_texture_slot(const std::string_view name) noexcept
    {
        if (auto i = texture_slots.find(name); i != end(texture_slots))
        {
            return std::get<uint>(*i);
        }

        auto slot = last_texture_slot;
        texture_slots[std::string{name}] = slot;
        last_texture_slot++;

        return slot;
    }

    int Shader::get_attribute(const std::string_view name) noexcept
    {
        GLOW_ASSERT(program_id != 0);
        auto id = glGetAttribLocation(program_id, name.data());
        GLOW_CHECK_GLERROR();
        return id;
    }

    void Shader::bind_output(const std::string_view name, uint channel) noexcept
    {
        GLOW_ASSERT(program_id != 0);
        glBindFragDataLocation(program_id, channel, name.data());
        GLOW_CHECK_GLERROR();
    }

}

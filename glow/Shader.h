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

#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <map>
#include <glm/glm.hpp>

#include "defines.h"
#include "Texture.h"

namespace glow
{
	//! All accepted uniform variable types.
    using UniformValue = std::variant<bool, int, uint, float,
                                      ivec2, uvec2, vec2,
                                      ivec3, uvec3, vec3,
                                      ivec4, uvec4, vec4,
                                      mat2,  mat3,  mat4>;

    //! GLSL Shader
    class GLOW_EXPORT Shader
    {
    public:
        Shader() noexcept = default;
        //! Create shader from code and compile it
        Shader(const std::string& code);
        ~Shader();

        //! Shader Code
        //!
        //! The shader code is one string with macros denoting the different parts.
        //!
        //! The following macros are currently used
        //!
        //! * GLOW_VERTEX: vertex program
        //! * GLOW_FRAGMENT: fragment program
        //!
        //! @{
        void set_code(const std::string& value) noexcept;
        const std::string& get_code() const noexcept;
        //! @}

        //! Compile the shader.
        void compile();

        //! Bind the shader.
        //!
        //! @note the shader must be compiled.
        void bind() noexcept;

        //! Unbind the shader.
        void unbind() noexcept;

        //! Set uniform variable.
        //!
        //! @param name the uniform variable name
        //! @param value the uniform value
        //!
        //! @note To bind textures first bind the texture to a given slot
        //! and then bind the slot as uint to the variable.
        void set_uniform(const std::string_view name, const UniformValue& value) noexcept;

        //! Bind texture to a uniform variable.
        //!
        //! This code will ensure that each texture gets a unique slot and
        //! will remain consisten from call to call.
        //!
        //! @param name the uniform variable name
        //! @param texture the texture to bind
        void set_uniform(const std::string_view name, Texture& texture) noexcept;

        //! Get the texture slot for the given variable name.
        uint get_texture_slot(const std::string_view name) noexcept;

        //! Get attribute index.
        //!
        //! @param name the variable name
        //! @return the index or -1
        int get_attribute(const std::string_view name) noexcept;

        //! Bind output to channel.
        //!
        //! @param name the variable name
        //! @param channel the chanel index.
        void bind_output(const std::string_view name, uint channel) noexcept;

    private:
        std::string  code;
        uint program_id = 0;

        uint last_texture_slot = 0u;
        std::map<std::string, uint, std::less<>> texture_slots;

        Shader(const Shader&) = delete;
        const Shader& operator = (const Shader&) = delete;
    };
}

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

#pragma once

#include <memory>
#include <variant>
#include <map>
#include <glm/fwd.hpp>

#include "defines.h"
#include "Shader.h"
#include "Texture.h"

namespace glow
{
    //! Set of Rendering Parameters
    //!
    //! The parameters can be eitehr a material, light or any context
    //! that needs parametes used for rendering.
    class GLOW_EXPORT Parameters
    {
    public:
        using Value = std::variant<bool, int, glm::uint, float,
                                   glm::ivec2, glm::uvec2, glm::vec2,
                                   glm::ivec3, glm::uvec3, glm::vec3,
                                   glm::ivec4, glm::uvec4, glm::vec4,
                                   glm::mat2, glm::mat3, glm::mat4,
                                   std::shared_ptr<Texture>>;

        //! Construct empty Parameters
        Parameters() noexcept = default;
        //! Initialize Parameters from map
        Parameters(const std::map<std::string, Value>& values) noexcept;
        //! Copy Parameters
        Parameters(const Parameters& orig) noexcept = default;
        //! Move Parameters
        Parameters(Parameters&& orig) noexcept = default;
        //! Destructor
        ~Parameters() = default;
        //! Assign Parameters
        Parameters& operator = (const Parameters& orig) noexcept = default;
        //! Move Assign Parameters
        Parameters& operator = (Parameters&& orig) noexcept = default;

        //! Set a parameter.
        void set_value(const std::string& id, const Value& value) noexcept;
        //! Check if a value is present.
        bool has_value(const std::string& id) const noexcept;
        //! Get a value.
        const Value& get_value(const std::string& id) const noexcept;

        //! Get all key value pairs in this parameter block.
        const std::map<std::string, Value>& get_values() const;

    private:
        std::map<std::string, Value> values;
    };

    GLOW_EXPORT void apply(Shader& shader, const Parameters& parameters) noexcept;

    inline std::shared_ptr<Parameters> make_shared_parameters(const std::map<std::string, Parameters::Value>& values) noexcept
    {
        return std::make_shared<Parameters>(values);
    }
}

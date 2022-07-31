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
#include "Parameters.h"

#include "util.h"

namespace glow
{
    Parameters::Parameters(const std::map<std::string, Value>& v) noexcept
    : values(v) {}

    void Parameters::set_value(const std::string& id, const Value& value) noexcept
    {
        values[id] = value;
    }

    bool Parameters::has_value(const std::string& id) const noexcept
    {
        return values.find(id) != end(values);
    }

    const Parameters::Value& Parameters::get_value(const std::string& id) const noexcept
    {
        auto i = values.find(id);
        GLOW_ASSERT(i != end(values));
        return i->second;
    }

    const std::map<std::string, Parameters::Value>& Parameters::get_values() const
    {
        return values;
    }

    void apply(Shader& shader, const Parameters& parmeters) noexcept
    {
        for (auto& [key, value] : parmeters.get_values())
        {
            std::visit(overloaded {
                [&] (const std::shared_ptr<Texture>& texture) { GLOW_ASSERT(texture); shader.set_uniform(key, *texture); },
                [&] (const auto& v)                           { shader.set_uniform(key, v);}
            }, value);
        }
    }
}

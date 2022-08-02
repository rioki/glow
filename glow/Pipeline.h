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
#include <glm/glm.hpp>

#include "defines.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Parameters.h"

namespace glow
{
    //! Pass Type
    enum class PassType
    {
        FULLSCREEN,          //!< once with full screen rect
        GEOMETRY,            //!< for each geometry
        LIGHTS,              //!< for each light
        LIGHTS_AND_GEOMETRY, //!< for each light for each geometry
        GEOMETRY_AND_LIGHTS  //!< for each geometry for each light
        // SHADOW            // TODO for each light but camera = light source
    };

    //! Depth Test
    enum class DepthTest
    {
        OFF,
        READ_ONLY,
        ON
    };

    //! Blending
    enum class Blending
    {
        OFF,
        ALPHA,
        MULTIPASS
    };

    //! Rendering Pipeline
    class GLOW_EXPORT Pipeline
    {
    public:
        Pipeline() noexcept = default;
        ~Pipeline() = default;


        //! Add a Pass
        //!
        //! @param type the type of render pass
        //! @param shader the shader to use
        //! @param depth_test the depth test algorithm to use
        //! @param blending the blending mode to use
        //! @param parameters additional shader parameters
        //!
        //! @{
        void add_pass(PassType group, const std::shared_ptr<Shader>& shader) noexcept;
        void add_pass(PassType group, const std::shared_ptr<Shader>& shader, DepthTest depth_test, Blending blending) noexcept;
        void add_pass(PassType group, const std::shared_ptr<Shader>& shader, DepthTest depth_test, Blending blending, const std::shared_ptr<Parameters>& parameters) noexcept;
        //! @}

        //! Set the camera matrixes
        void set_camera(const glm::mat4& projection, const glm::mat4& view) noexcept;

        //! Add geometry.
        unsigned int add_geometry(const glm::mat4& transform, const std::shared_ptr<VertexBuffer>& mesh, const std::shared_ptr<Parameters>& parameters) noexcept;

        //! Update geometry
        //!
        //! @{
        void update_geometry(unsigned int id, const glm::mat4& transform) noexcept;
        void update_geometry(unsigned int id, const std::shared_ptr<VertexBuffer>& mesh) noexcept;
        void update_geometry(unsigned int id, const std::shared_ptr<Parameters>& parameters) noexcept;
        void remove_geometry(unsigned int id) noexcept;
        //! @}

        //! Remove geometry.
        void remove_all_geometry() noexcept;

        //! Add light
        unsigned int add_light(const std::shared_ptr<Parameters>& parameters) noexcept;

        //! Update light
        //!
        //! @{
        void update_light(unsigned int id, const std::shared_ptr<Parameters>& parameters) noexcept;
        void remove_light(unsigned int id) noexcept;

        //! Remove light
        void remove_all_light() noexcept;

        //! Execute the pipeline
        void execute() noexcept;

    private:
        struct Pass
        {
            PassType                    group;
            std::shared_ptr<Shader>     shader;
            DepthTest                   depth_test;
            Blending                    blending;
            std::shared_ptr<Parameters> parameters;
        };

        struct Geometry
        {
            glm::mat4                     transform;
            std::shared_ptr<VertexBuffer> mesh;
            std::shared_ptr<Parameters>   parameters;
        };

        struct Light
        {
            std::shared_ptr<Parameters> parameters;
        };

        std::unique_ptr<VertexBuffer> fullscreen_rectangle = make_fullscreen_rectangle();

        std::vector<Pass> passes;

        glm::mat4 projection_matrix = glm::mat4(1.0f);
        glm::mat4 view_matrix       = glm::mat4(1.0f);

        unsigned int last_geom_id = 0;
        std::map<unsigned int, Geometry> geoms;
        unsigned int last_light_id = 0;
        std::map<unsigned int, Light> lights;

        static std::unique_ptr<VertexBuffer> make_fullscreen_rectangle() noexcept;

        Pipeline(const Pipeline& orig) = delete;
        Pipeline& operator = (const Pipeline& orig) = delete;

        friend void apply(Shader& shader, Light& light) noexcept;
        friend void apply(Shader& shader, Geometry& geom) noexcept;
    };
}

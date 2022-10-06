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
#include "Pipeline.h"

namespace glow
{
    void Pipeline::add_pass(PassType group, const std::shared_ptr<Shader>& shader) noexcept
    {
        DepthTest depth_test;
        Blending  blending;
        switch (group)
        {
        case PassType::FULLSCREEN:
            depth_test = DepthTest::OFF;
            blending   = Blending::OFF;
            break;
        case PassType::GEOMETRY:
            depth_test = DepthTest::ON;
            blending   = Blending::ALPHA;
            break;
        case PassType::LIGHTS:
            depth_test = DepthTest::OFF;
            blending   = Blending::OFF;
            break;
        case PassType::LIGHTS_AND_GEOMETRY:
        case PassType::GEOMETRY_AND_LIGHTS:
            depth_test = DepthTest::ON;
            blending   = Blending::MULTIPASS;
            break;
        }

        add_pass(group, shader, depth_test, blending, std::make_shared<Parameters>());
    }

    void Pipeline::add_pass(PassType group, const std::shared_ptr<Shader>& shader, DepthTest depth_test, Blending blending) noexcept
    {
        add_pass(group, shader, depth_test, blending, std::make_shared<Parameters>());
    }

    void Pipeline::add_pass(PassType group, const std::shared_ptr<Shader>& shader, DepthTest depth_test, Blending blending, const std::shared_ptr<Parameters>& parameters) noexcept
    {
        GLOW_ASSERT(shader);
        GLOW_ASSERT(parameters);
        passes.push_back({group, shader, depth_test, blending, parameters});
    }

    void Pipeline::set_camera(const glm::mat4& projection, const glm::mat4& view) noexcept
    {
        projection_matrix = projection;
        view_matrix = view;
    }

    unsigned int Pipeline::add_geometry(const glm::mat4& transform, const std::shared_ptr<VertexBuffer>& mesh, const std::shared_ptr<Parameters>& parameters) noexcept
    {
        GLOW_ASSERT(mesh);
        GLOW_ASSERT(parameters);
        last_geom_id++;
        geoms[last_geom_id] = {transform, mesh, parameters};
        return last_geom_id;
    }

    void Pipeline::update_geometry(unsigned int id, const glm::mat4& transform) noexcept
    {
        auto i = geoms.find(id);
        GLOW_ASSERT(i != end(geoms));
        i->second.transform = transform;
    }

    void Pipeline::update_geometry(unsigned int id, const std::shared_ptr<VertexBuffer>& mesh) noexcept
    {
        auto i = geoms.find(id);
        GLOW_ASSERT(i != end(geoms));
        i->second.mesh = mesh;
    }

    void Pipeline::update_geometry(unsigned int id, const std::shared_ptr<Parameters>& parameters) noexcept
    {
        auto i = geoms.find(id);
        GLOW_ASSERT(i != end(geoms));
        i->second.parameters = parameters;
    }

    void Pipeline::remove_geometry(unsigned int id) noexcept
    {
        auto i = geoms.find(id);
        GLOW_ASSERT(i != end(geoms));
        geoms.erase(i);
    }

    void Pipeline::remove_all_geometry() noexcept
    {
        last_geom_id = 0;
        geoms.clear();
    }

    unsigned int Pipeline::add_light(const std::shared_ptr<Parameters>& parameters) noexcept
    {
        last_light_id++;
        lights[last_light_id] = {parameters};
        return last_light_id;
    }

    void Pipeline::update_light(unsigned int id, const std::shared_ptr<Parameters>& parameters) noexcept
    {
        auto i = lights.find(id);
        GLOW_ASSERT(i != end(lights));
        i->second.parameters = parameters;
    }

    void Pipeline::remove_light(unsigned int id) noexcept
    {
        auto i = lights.find(id);
        GLOW_ASSERT(i != end(lights));
        lights.erase(i);
    }

    void Pipeline::remove_all_light() noexcept
    {
        last_light_id = 0;
        lights.clear();
    }

    void apply(Shader& shader, Pipeline::Light& light) noexcept
    {
        GLOW_ASSERT(light.parameters);
        apply(shader, *light.parameters);
    }

    void apply(Shader& shader, Pipeline::Geometry& geom) noexcept
    {
        shader.set_uniform("glow_ModelMatrix", geom.transform);
        GLOW_ASSERT(geom.parameters);
        apply(shader, *geom.parameters);
    }

    void apply(DepthTest depth_test) noexcept
    {
        switch (depth_test)
        {
        case DepthTest::OFF:
            glDisable(GL_DEPTH_TEST);
            break;
        case DepthTest::READ_ONLY:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);
            break;
        case DepthTest::ON:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_TRUE);
            break;
        }
    }

    void apply(Blending blending, bool& first_multipass) noexcept
    {
        switch (blending)
        {
        case Blending::OFF:
            glDisable(GL_BLEND);
            break;
        case Blending::ALPHA:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case Blending::MULTIPASS:
            if (first_multipass)
            {
                glDisable(GL_BLEND);
                first_multipass = false;
            }
            else
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            }
            break;
        }
    }

    void Pipeline::execute() noexcept
    {
        auto first_multipass = true;
        for (auto& pass : passes)
        {
            pass.shader->bind();
            pass.shader->bind_output("glow_FragColor", 0);
            pass.shader->set_uniform("glow_ProjectionMatrix", projection_matrix);
            pass.shader->set_uniform("glow_ViewMatrix",       view_matrix);
            apply(*pass.shader, *pass.parameters);
            apply(pass.depth_test);
            switch (pass.group)
            {
            case PassType::FULLSCREEN:
                apply(pass.blending, first_multipass);
                fullscreen_rectangle->bind(*pass.shader);
                fullscreen_rectangle->draw();
                break;
            case PassType::GEOMETRY:
                for (auto& [id, geom] : geoms)
                {
                    apply(pass.blending, first_multipass);
                    apply(*pass.shader, geom);
                    geom.mesh->bind(*pass.shader);
                    geom.mesh->draw();
                }
                break;
            case PassType::LIGHTS:
                for (auto& [id, light] : lights)
                {
                    apply(pass.blending, first_multipass);
                    apply(*pass.shader, light);
                    fullscreen_rectangle->bind(*pass.shader);
                    fullscreen_rectangle->draw();
                }
                break;
            case PassType::LIGHTS_AND_GEOMETRY:
                for (auto& [id, light] : lights)
                {
                    apply(*pass.shader, light);
                    apply(pass.blending, first_multipass);
                    for (auto& [id, geom] : geoms)
                    {
                        apply(*pass.shader, geom);
                        geom.mesh->bind(*pass.shader);
                        geom.mesh->draw();

                    }
                }
                break;
            case PassType::GEOMETRY_AND_LIGHTS:
                for (auto& [id, geom] : geoms)
                {
                    auto tmp = first_multipass;
                    apply(*pass.shader, geom);
                    geom.mesh->bind(*pass.shader);
                    for (auto& [id, light] : lights)
                    {
                        apply(*pass.shader, light);
                        apply(pass.blending, tmp);
                        geom.mesh->draw();
                    }
                }
                if (pass.blending == Blending::MULTIPASS)
                {
                    first_multipass = false;
                }
                break;
            }
        }
    }

    std::unique_ptr<VertexBuffer> Pipeline::make_fullscreen_rectangle() noexcept
    {
        auto vertexes = std::vector<glm::vec3>{
            {-1.0f, -1.0f, 0.0f},
            {-1.0f,  1.0f, 0.0f},
            { 1.0f,  1.0f, 0.0f},
            { 1.0f, -1.0f, 0.0f}
        };

        auto textcoord = std::vector<glm::vec2>{
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f}
        };

        auto indexes = std::vector<glm::uvec3>{
            {0, 1, 2},
            {2, 3, 0}
        };

        auto mesh = std::make_unique<VertexBuffer>();
        mesh->upload_values(VERTEX, vertexes);
        mesh->upload_values(TEXCOORD, textcoord);
        mesh->upload_indexes(indexes);
        return mesh;
    }
}

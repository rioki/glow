
#include "Shader.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "util.h"

namespace glow
{
    Shader::Shader()
    : bound(false), program_id(0) {}

    Shader::~Shader() 
    {
        if (program_id != 0)
        {
            glDeleteProgram(program_id);
            program_id = 0;

            assert(glGetError() == GL_NO_ERROR);
        }
    }

    void Shader::set_vertex_code(const std::string& value)
    {
        vertex_code = value;    
    }

    const std::string& Shader::get_vertex_code() const
    {
        return vertex_code;    
    }

    void Shader::set_fragment_code(const std::string& value)
    {
        fragment_code = value;
    }

    const std::string& Shader::get_fragment_code() const
    {
        return fragment_code;    
    }

    void Shader::compile()
    {
        assert(program_id == 0);
        
        int status = 0;
        char logstr[256];

        const GLchar* vbuff[1] = { vertex_code.c_str() };

        unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_id, 1, vbuff, NULL);
        glCompileShader(vertex_id);

        glGetShaderInfoLog(vertex_id, 256, NULL, logstr);        

        glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            throw std::runtime_error(logstr);
        }

        const GLchar* fbuff[1] = { fragment_code.c_str() };

        unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_id, 1, fbuff, NULL);
        glCompileShader(fragment_id);

        glGetShaderInfoLog(fragment_id, 256, NULL, logstr);

        glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);
            throw std::runtime_error(logstr);
        }

        assert(glGetError() == GL_NO_ERROR);

        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_id);
        assert(glGetError() == GL_NO_ERROR);
        glAttachShader(program_id, fragment_id);
        assert(glGetError() == GL_NO_ERROR);
        glLinkProgram(program_id);

        glGetProgramInfoLog(program_id, 256, NULL, logstr);
        
        glGetProgramiv(program_id, GL_LINK_STATUS, &status);
        if (!status)
        {
            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);
            glDeleteProgram(program_id);
            throw std::runtime_error(logstr);
        }

        // NOTE: glDeleteShader() actually does not delete the shader, it only
        // flags the shader for deletion. The shaders will be deleted when
        // the program gets deleted.
        glDeleteShader(vertex_id);
        glDeleteShader(fragment_id);

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::bind()
    {
        assert(program_id != 0);
        
        glUseProgram(program_id);
        bound = true;

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::unbind()
    {
        glUseProgram(0);
        bound = false;

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniformi(const std::string & name, int value)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform1i(location, value);            
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniformf(const std::string & name, float value)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform1f(location, value);            
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform2i(const std::string & name, int x, int y)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform2i(location, x, y);            
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform2f(const std::string & name, float x, float y)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform2f(location, x, y);            
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform3i(const std::string & name, int x, int y, int z)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform3i(location, x, y, z);  
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform3f(const std::string & name, float x, float y, float z)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform3f(location, x, y, z);            
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform4i(const std::string & name, int x, int y, int z, int m)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform4i(location, x, y, z, m);
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform4f(const std::string & name, float x, float y, float z, float m)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniform4f(location, x, y, z, m); 
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform_matrix2(const std::string & name, const float* value)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniformMatrix2fv(location, 1, GL_FALSE, value);
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform_matrix3(const std::string & name, const float* value)
    {
        assert(bound);
        
        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, value);
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniform_matrix4(const std::string & name, const float* value)
    {
        assert(bound);
        
        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, value);
        }

        assert(glGetError() == GL_NO_ERROR);
    }

    void Shader::set_uniformiv(const std::string& name, const std::vector<int>& value)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            switch (value.size())
            {
                case 1:
                    glUniform1iv(location, 1, &value[0]);    
                    break;
                case 2:
                    glUniform2iv(location, 1, &value[0]);
                    break;
                case 3:
                    glUniform3iv(location, 1, &value[0]);
                    break;
                case 4:
                    glUniform4iv(location, 1, &value[0]);
                    break;
                default:     
                    assert(false && "Invalid value array");               
                    break;
            }            
        }  
        
        assert(glGetError() == GL_NO_ERROR);      
    }
    
    void Shader::set_uniformfv(const std::string& name, const std::vector<float>& value)
    {
        assert(bound);

        int location = glGetUniformLocation(program_id, name.c_str());
        if (location != -1)
        {
            switch (value.size())
            {
                case 1:
                    glUniform1fv(location, 1, &value[0]);    
                    break;
                case 2:
                    glUniform2fv(location, 1, &value[0]);
                    break;
                case 3:
                    glUniform3fv(location, 1, &value[0]);
                    break;
                case 4:
                    glUniform4fv(location, 1, &value[0]);
                    break;
                case 9:
                    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0]);
                    break;
                case 16:
                    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0]);
                    break;
                default:  
                    assert(false && "Invalid value array");                  
                    break;
            }            
        }  
        
        assert(glGetError() == GL_NO_ERROR);      
    }

    unsigned int Shader::get_attribute(const std::string & name)
    {
        assert(bound);

        unsigned int id = glGetAttribLocation(program_id, name.c_str());

        assert(glGetError() == GL_NO_ERROR);

        return id;
    }   

    void Shader::bind_output(const std::string& name, unsigned int channel)
    {
        assert(bound);

        glBindFragDataLocation(program_id, channel, name.c_str());

        assert(glGetError() == GL_NO_ERROR);
    }

}
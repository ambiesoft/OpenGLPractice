
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include <GL/glew.h>

#include "Renderer.h"

#include "Shader.h"



Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_RedererID(0)
{
    ShaderProgramSources source = ParseShader();
    //std::cout << "VERTEX >>>>" << '\n';
    //std::cout << source.VertexSource << '\n';
    //std::cout << "<<<< VERTEX" << '\n';

    //std::cout << "FRAGMENT >>>>" << '\n';
    //std::cout << source.FragmentSource << '\n';
    //std::cout << "<<<< FRAGMENT" << '\n';

    m_RedererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
    CALLGL(glDeleteProgram(m_RedererID));
}



ShaderProgramSources Shader::ParseShader()
{
    std::ifstream stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX,
        FRAGMENT,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    CALLGL(glShaderSource(id,
        1, // count
        &src,
        nullptr));
    CALLGL(glCompileShader(id));

    // TODO: Error handling
    int result;
    CALLGL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        int length;
        CALLGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> message;
        message.resize(length + 1);
        CALLGL(glGetShaderInfoLog(id, length, &length, &message[0]));
        std::cerr << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            " shader!" << '\n';
        std::cerr << &message[0] << '\n';

        CALLGL(glDeleteShader(id));
        return 0;
    }
    return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    CALLGL(glAttachShader(program, vs));
    CALLGL(glAttachShader(program, fs));

    CALLGL(glLinkProgram(program));
    CALLGL(glValidateProgram(program));

    CALLGL(glDeleteShader(vs));
    CALLGL(glDeleteShader(fs));

    return program;
}


void Shader::Bind() const
{
    CALLGL(glUseProgram(m_RedererID));
}
void Shader::Unbind() const
{
    CALLGL(glUseProgram(0));
}

// set uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
    CALLGL(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    CALLGL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    CALLGL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RedererID, name.c_str());
    if (location == -1)
        std::cerr << "Warning uniform '" << name << "' doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}
#include "Shader.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    :m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    Initialized = true;
}

Shader::~Shader()
{
    if (Initialized && !Moved)
    {
        GLCall(glDeleteProgram(m_RendererID));
    }
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

void Shader::Bind() const
{
    if (Initialized)
    {
        GLCall(glUseProgram(m_RendererID));
    }
}

void Shader::Unbind() const
{
    if (Initialized)
    {
        GLCall(glUseProgram(0));
    }
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    this->m_RendererID = other.m_RendererID;
    this->m_UniformLocationCache = other.m_UniformLocationCache;
    this->m_FilePath = other.m_FilePath;
    this->Initialized = true;
    other.Moved = true; //prevents deletion of the ID because OpenGL hates Object Oriented Programming
    //other.~Shader(); //shaders for some whatever reason hate this

    return *this;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    if (Initialized)
    {
        GLCall(glUniform1i(GetUniformLocation(name), value));
    }
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    if (Initialized)
    {
        GLCall(glUniform1f(GetUniformLocation(name), value));
    }
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    if (Initialized)
    {
        GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    if (Initialized)
    {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }
}

void Shader::SetUniform1iv(const std::string& name, int amount, int arr[])
{
    if (Initialized)
    {
        GLCall(glUniform1iv(GetUniformLocation(name), amount, arr));
    }
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    if (Initialized)
    {
        GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform" << name << " doesnt exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

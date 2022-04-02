#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"


Shader::Shader(const std::string& filePath)
	: m_filePath(filePath), m_RendererID(0)
{
    ShaderProgramSource shaderSource = this->parseShader();
    this->createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
}

Shader::~Shader()
{
    GlCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    GlCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    GlCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int v0)
{
    int location = this->getUniformLocation(name.c_str());
    GlCall(glUniform1i(location, v0));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = this->getUniformLocation(name.c_str());
    GlCall(glUniform4f(location, v0, v1, v2, v3));
}


ShaderProgramSource Shader::parseShader()
{
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    std::ifstream stream(m_filePath); // open file
    ShaderType type = ShaderType::NONE; // initial non
    std::stringstream ss[3];
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            // if shader found update Type variable
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;
        }
        else {
            // otherwise write to stringStream unless type is -1
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }
    return {
        ss[(int)ShaderType::VERTEX].str(),
        ss[(int)ShaderType::FRAGMENT].str(),
        ss[(int)ShaderType::GEOMETRY].str()
    };
}


unsigned int Shader::compileShader(GLenum type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int results;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &results));
    if (results == GL_FALSE) {

        int len;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        char* message = (char*)malloc(len * sizeof(char));
        GlCall(glGetShaderInfoLog(id, len, &len, message));

        std::cout << "Failed to compile " << (shaderType2Str(type)) << std::endl;
        std::cout << message << std::endl;
    }
    return id;
}

void Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    m_RendererID = glCreateProgram();
    unsigned int vs = this->compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = this->compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GlCall(glAttachShader(m_RendererID, vs));
    GlCall(glAttachShader(m_RendererID, fs));
    GlCall(glLinkProgram(m_RendererID));
    GlCall(glValidateProgram(m_RendererID));

    GlCall(glDetachShader(m_RendererID, vs));
    GlCall(glDetachShader(m_RendererID, fs));
    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));
}


unsigned int Shader::getUniformLocation(const char* name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name);
    if (location == -1)
        std::cout << "Warning: Uniform " << name << "does not exists !" << std::endl;
    m_uniformLocationCache[name] = location;
    return location;
}


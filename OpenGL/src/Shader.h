#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
private:
	std::string m_filePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int v0);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	inline const char* shaderType2Str(GLenum shader)
	{
		switch (shader) {
		case GL_VERTEX_SHADER: return "Vertex Shader";
		case GL_FRAGMENT_SHADER: return "Fragment Shader";
		case GL_GEOMETRY_SHADER: return "Geometry Shader";
		default: return "";
		}
	}
	
	ShaderProgramSource parseShader();
	unsigned int compileShader(GLenum type, const std::string& source);
	void createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int getUniformLocation(const char* name);

};


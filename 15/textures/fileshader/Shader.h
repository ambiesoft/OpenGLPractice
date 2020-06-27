#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSources
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RedererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSources ParseShader();
	unsigned int CompileShader(GLenum type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};
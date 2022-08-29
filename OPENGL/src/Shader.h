#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	
	//Chaching for uniforms
	std::unordered_map<std::string, int> m_UniformCache;

public:

	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Setting Uniforms

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	bool CompileShader();


	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);


	void SetUniformLocation(const std::string& UniformName);
	
		


};

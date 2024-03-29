#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

typedef unsigned int uint;

class Shader
{
private:
	uint	_rendererId;
	std::unordered_map<std::string, int>	_uniformLocationCache;

public:
	Shader(const std::string& vertex, const std::string& fragment);
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	~Shader();
	void	bind() const;
	void	unbind() const;

	int		getUniformLocation(const std::string& name);

	void	setUniform1i(const std::string& name, int value);
	void	setUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	void	setUniform2f(const std::string& name, float v1, float v2);
	void	setUniform2i(const std::string& name, int v1, int v2);
	void	setUniform3i(const std::string& name, int v1, int v2, int v3);
	void	setUniform3i(int location, int v1, int v2, int v3);
};

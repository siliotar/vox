#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "OpenGL.hpp"

static void readFile(const std::string& filePath, std::stringstream& stream)
{
	std::ifstream	fstream(filePath);
	std::string		line;

	while (getline(fstream, line))
		stream << line << "\n";
	fstream.close();
}

static uint	compileShader(unsigned int type, const std::string& source)
{
	unsigned int	id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	int	result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int	length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char	message[512];
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << "Failed to compile shader\n" << message << std::endl;
		glDeleteShader(id);
		exit(EXIT_FAILURE);
	}
	return id;
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::stringstream	vertexSStream;
	std::stringstream	fragmentSStream;

	readFile(vertexShaderPath, vertexSStream);
	readFile(fragmentShaderPath, fragmentSStream);

	_rendererId = glCreateProgram();
	uint	vs = compileShader(GL_VERTEX_SHADER, vertexSStream.str());
	uint	fs = compileShader(GL_FRAGMENT_SHADER, fragmentSStream.str());

	glAttachShader(_rendererId, vs);
	glAttachShader(_rendererId, fs);
	glLinkProgram(_rendererId);
	glValidateProgram(_rendererId);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(_rendererId);
}

void	Shader::bind() const
{
	glUseProgram(_rendererId);
}

void	Shader::unbind() const
{
	glUseProgram(0);
}

int		Shader::getUniformLocation(const std::string& name)
{
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		return _uniformLocationCache[name];

	int location = glGetUniformLocation(_rendererId, name.c_str());
	if (location == -1)
		std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

	_uniformLocationCache[name] = location;
	return location;
}

void	Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void	Shader::setUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void	Shader::setUniform2f(const std::string& name, float v1, float v2)
{
	glUniform2f(getUniformLocation(name), v1, v2);
}

void	Shader::setUniform2i(const std::string& name, int v1, int v2)
{
	glUniform2i(getUniformLocation(name), v1, v2);
}

void	Shader::setUniform3i(const std::string& name, int v1, int v2, int v3)
{
	glUniform3i(getUniformLocation(name), v1, v2, v3);
}

void	Shader::setUniform3i(int location, int v1, int v2, int v3)
{
	glUniform3i(location, v1, v2, v3);
}

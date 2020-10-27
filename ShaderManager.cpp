#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "const.h"
#include "ShaderManager.h"

unsigned int ShaderManager::shaders = 0;
SSet ShaderManager::shaderSets[MAX_SHADER_SETS] = { 0 };

SSet newShaderSet(const std::string& path)
{
	std::ifstream stream(path);
	std::string line;
	std::stringstream srcs[2];
	ShaderType currType;
	SSet shaders;
	if ((shaders = (SSet)calloc(1, sizeof(struct ShaderSet))) == NULL)
	{
		std::cout << "newShaderSet(): Failed to allocate space for new shader set" << std::endl;
	}
	while (std::getline(stream, line))
	{
		if (line.find(SHADER_KEYWORD) != std::string::npos)
		{
			if (line.find(VS_KEYWORD) != std::string::npos)
			{
				currType = VERTEX;
			}
			else if (line.find(FS_KEYWORD) != std::string::npos)
			{
				currType = FRAGMENT;
			}
			else
			{
				std::cout << "ShaderSet(): Undefined shader type" << std::endl;
			}
		}
		else
		{
			switch (currType)
			{
				case VERTEX:
					srcs[VERTEX] << line << "\n";
					break;
				case FRAGMENT:
					srcs[FRAGMENT] << line << "\n";
					break;
				default:
					std::cout << "ShaderSet(): No declaration of shader found" << std::endl;
					break;
			}
		}
	}
	shaders->vertexShader = srcs[VERTEX].str();
	shaders->fragmentShader = srcs[FRAGMENT].str();
	return shaders;
}

int ShaderManager::loadShaders(SSet source)
{
	unsigned int programID = glCreateProgram();
	//TODO: Make dynamic
	if (createShader(programID, GL_VERTEX_SHADER, source->vertexShader.c_str()) != OK ||
		createShader(programID, GL_FRAGMENT_SHADER, source->fragmentShader.c_str()) != OK)
	{
		return -1;
	}
	glUseProgram(programID);
	source->programID = programID;
	shaderSets[this->shaders++] = source;
	return 0;
}

int ShaderManager::createShader(unsigned int programID, unsigned int type, const char* source)
{
	unsigned int shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);
	// TODO : Error message
	glAttachShader(programID, shaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);
	glDeleteShader(shaderID);
	return 0;
}

void ShaderManager::close()
{
	for (unsigned int i = 0; i < shaders; i++)
	{
		glDeleteProgram(this->shaderSets[i]->programID);
		free(this->shaderSets[i]);
	}
}
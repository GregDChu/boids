#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "const.h"
#include "VariableManager.h"

unsigned int VariableManager::vecs = 0;
float *VariableManager::vecData[MAX_VEC_SLOTS] = { 0 };
int VariableManager::vecLoc[MAX_VEC_SLOTS] = { -1 };
unsigned int VariableManager::mats = 0;
glm::mat4 *VariableManager::matData[MAX_MAT_SLOTS] = { 0 };
int VariableManager::matLoc[MAX_MAT_SLOTS] = { -1 };

int VariableManager::addGlobalVec(unsigned int shaderID, const char *name, float *vec)
{
	vecLoc[vecs] = glGetUniformLocation(shaderID, name);
	if (vecLoc[vecs] == -1)
	{
		std::cout << "Warning: Uniform " << name << " was not used in shader " << shaderID << std::endl;
		return -1;
	}
	else
	{
		vecData[vecs] = vec;
		glUniform4f(vecLoc[vecs], vec[0], vec[1], vec[2], vec[3]);
	}
	return vecs++;
}

int VariableManager::updateGlobalVec(int index, float *vec)
{
	if (index >= MAX_VEC_SLOTS)
	{
		std::cout << "updateGlobalVec(): Out of bounds global at index " << index << std::endl;
		return -1;
	}
	vecData[index] = vec;
	glUniform4f(vecLoc[index], vec[0], vec[1], vec[2], vec[3]);
	return OK;
}


int VariableManager::addGlobalMat(unsigned int shaderID, const char *name, glm::mat4 mat)
{
	matLoc[mats] = glGetUniformLocation(shaderID, name);
	if (matLoc[mats] == -1)
	{
		std::cout << "Warning: Uniform " << name << " was not used in shader " << shaderID << std::endl;
		return -1;
	}
	else
	{
		matData[mats] = &mat;
		glProgramUniformMatrix4fv(shaderID, matLoc[mats], 1, GL_FALSE, (float*)&mat[0]);
	}
	return mats++;
}

int VariableManager::updateGlobalMat(int index, glm::mat4 mat)
{
	if (index >= MAX_MAT_SLOTS)
	{
		std::cout << "updateGlobalVec(): Out of bounds global at index " << index << std::endl;
		return -1;
	}
	matData[index] = &mat;
	glUniformMatrix4fv(matLoc[index], 1, GL_FALSE, (float*)&mat[0]);
	return OK;
}
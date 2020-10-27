#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include "VertexManager.h"

using namespace std;

unsigned int VertexManager::bufferID[DEFAULT_BUFFER_SLOTS] = { 0 };
VGroup VertexManager::vertGroups[DEFAULT_BUFFER_SLOTS] = { 0 };
int VertexManager::activeBuffers = 0;

VGroup newVGroup(unsigned int vcount, void *data)
{
	VGroup group;
	if ((group = (VGroup)calloc(1, sizeof(struct VertexGroup))) == NULL)
	{
		std::cout << "newVGroup(): Cannot allocate memory for group" << std::endl;
		return NULL;
	}
	group->elements = vcount * VERTEX_DIMENSIONS;
	group->data = data;
	return group;
}

VGroup newIGroup(unsigned int icount, void *data)
{
	VGroup group;
	if ((group = (VGroup)calloc(1, sizeof(struct VertexGroup))) == NULL)
	{
		std::cout << "newVGroup(): Cannot allocate memory for group" << std::endl;
		return NULL;
	}
	group->elements = icount;
	group->data = data;
	return group;
}

int defineAttribute(VGroup group, AttribType type, unsigned int elements)
{
	if (group == NULL)
	{
		std::cout << "defineAttribute(): Null pointer given for vertex group" << std::endl;
		return -1;
	}
	group->attribTypes[group->attribs] = type;
	if (type != INDEX)
	{
		group->attribGroupSize[group->attribs] = elements * VERTEX_DIMENSIONS;
	}
	group->attribs++;// TODO: Update so attribs is static!
	return 0;
}

int VertexManager::loadVertGroup(VGroup group)
{
	/* Generate and bind buffer to ID */
	glGenBuffers(1, &(bufferID[activeBuffers]));
	glBindBuffer(GL_ARRAY_BUFFER, bufferID[activeBuffers]);
	/* Define buffer data space and content */
	glBufferData(GL_ARRAY_BUFFER, group->elements * sizeof(float), group->data,
				 GL_STATIC_DRAW);
	/* Init VAO instance*/
	
	/* Define buffer data attributes*/
	unsigned int currAttribOffset = 0;
	for (unsigned int i = 0; i < group->attribs; i++)
	{
		// TODO: Change this to have a recurring pattern of attributes
		// allows for dynamic scaling

		/* Assume each attribute is a 4D float and
		all data for each attribute type is
		packed together as discrete groups*/
		glVertexAttribPointer(i, VERTEX_DIMENSIONS, GL_FLOAT, GL_FALSE,
				                VERTEX_DIMENSIONS * sizeof(float),
				                (const void*)currAttribOffset);
		glEnableVertexAttribArray(i);
		/* Add size of current data chunk to offset for next attrib */
		currAttribOffset += (group->attribGroupSize[i] * sizeof(float));
	}
	/* Update buffer count */
	this->activeBuffers++;
	return 0;
}

int VertexManager::loadIndexGroup(VGroup group)
{
	/* Generate and bind index buffer to ID */
	glGenBuffers(1, &(bufferID[activeBuffers]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID[activeBuffers]);
	/* Define buffer data space and content */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, group->elements * sizeof(unsigned int), group->data,
		GL_STATIC_DRAW);
	/* Update buffer count */
	this->activeBuffers++;
	return 0;
}

void VertexManager::close()
{
	// TODO: Fix magic number 10
	for (int i = 0; i < this->activeBuffers; i++)
	{
		free(this->vertGroups[i]);
	}
}
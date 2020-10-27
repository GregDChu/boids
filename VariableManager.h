#ifndef H_VARIABLE_MANAGER
#define H_VARIABLE_MANAGER

#include <string>
#include "glm/glm.hpp"

#define MAX_VEC_SLOTS 10
#define MAX_MAT_SLOTS 10

class VariableManager
{
public:
	// Vector variables
	static unsigned int vecs;
	static float *vecData[MAX_VEC_SLOTS];
	static int vecLoc[MAX_VEC_SLOTS];
	int addGlobalVec(unsigned int shaderID, const char *name, float *vec);
	int updateGlobalVec(int index, float *vec);
	// Matrix variables
	static unsigned int mats;
	static glm::mat4 *matData[MAX_MAT_SLOTS];
	static int matLoc[MAX_MAT_SLOTS];
	int addGlobalMat(unsigned int shaderID, const char *name, glm::mat4 mat);
	int updateGlobalMat(int index, glm::mat4 mat);
};

#endif
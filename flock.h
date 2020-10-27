#ifndef H_FLOCK
#define H_FLOCK

#include "glm/glm.hpp"

#define AWARE_RADIUS 0.5
#define AWARE_DEGREE 0.75

#define AVOID_INFLUENCE 1.0
#define ALIGN_INFLUENCE 1.0
#define ATTRACT_INFLUENCE 1.0

#define VARIABLE_SPEEDS 0
#define MAX_SPEED 1.0
#define SPEED_FACTOR 1.0

#define FLOCK_SIZE 100

#define AVOID 0
#define ALIGN 2
#define ATTRACT 1

#define RADIUS 0
#define DEGREE 1

typedef struct FoidStruct &Foid;
typedef struct FoidStruct
{
	glm::vec3 pos;
	glm::vec3 dir;
	float speed;
	glm::vec4 color;
} &Foid;

class Flock
{
public:
	//static Foid population[FLOCK_SIZE];
	static struct FoidStruct population[FLOCK_SIZE];
	static glm::vec3 dirUpdate[FLOCK_SIZE][3];
	static bool speedVari;
	static int flockSize;
	static float speedFactor;
	static glm::vec2 awareVec;
	static glm::vec3 behavInfluence;
	void populateFlock();
	void freeFlock();
	void calcBehavior(float avoid, float align, float attract);
	glm::mat4 getModelMat(unsigned int index);
	void update();
};

#endif

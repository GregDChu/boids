#include <iostream>
#include <time.h>
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "const.h"
#include "flock.h"

struct FoidStruct Flock::population[FLOCK_SIZE] = {
	glm::vec3(0.0,0.0,0.0),
	glm::vec3(0.0,0.0,0.0), 
	0.0,
	glm::vec4(1.0, 1.0, 1.0, 1.0)};
glm::vec3 Flock::dirUpdate[FLOCK_SIZE][3] = { {
	glm::vec3(0, 0, 0)
} };

bool Flock::speedVari = VARIABLE_SPEEDS;
int Flock::flockSize = FLOCK_SIZE;
float Flock::speedFactor = SPEED_FACTOR;
glm::vec2 Flock::awareVec = glm::vec2(AWARE_RADIUS, AWARE_DEGREE);
glm::vec3 Flock::behavInfluence = glm::vec3(AVOID_INFLUENCE, ALIGN_INFLUENCE, ATTRACT_INFLUENCE);

glm::vec3 randVec3(bool normalize)
{
	float x, y, z;
	glm::vec3 randvec;
	randvec[0] = (((float)rand() / RAND_MAX) * 2.0) - 1.0;
	randvec[1] = (((float)rand() / RAND_MAX) * 2.0) - 1.0;
	randvec[2] = 0.0;
	// Ignore this for now, only calculating for 2D
	//randvec[2] = (float)rand() / RAND_MAX * 2.0 - 1.0;
	return normalize ? glm::normalize(randvec) : randvec;
}

void Flock::populateFlock()
{
	srand(time(NULL));
	for(int i = 0; i < FLOCK_SIZE; i++)
	{
		struct FoidStruct& foid = this->population[i];
		foid.pos = randVec3(false);
		foid.dir = randVec3(true);
		foid.speed = ((float)rand() * MAX_SPEED) / RAND_MAX;
		foid.color = glm::vec4(
			((float)rand() / RAND_MAX * 0.6) + 0.4,
			((float)rand() / RAND_MAX * 0.6) + 0.4,
			((float)rand() / RAND_MAX * 0.6) + 0.4,
			1);
	}
}

void Flock::update()
{
	static clock_t lastUpdate = 0;
	static clock_t nextUpdate = 0;
	float elapsedTime;
	nextUpdate = clock();
	elapsedTime = ((float)nextUpdate - lastUpdate) / CLOCKS_PER_SEC;
	lastUpdate = clock();
	/* Any extreme lag, refuse to update position*/
	if (elapsedTime > 1.0)
	{
		elapsedTime = 0;
	}
	/* Update position */
	for (int i = 0; i < FLOCK_SIZE; i++)
	{
		struct FoidStruct& foid = this->population[i];
		foid.dir = glm::normalize(
			foid.dir +
			dirUpdate[i][AVOID] + // TODO: Fix jittering of foid heading, caused by bounce from avoiding neighbors
			dirUpdate[i][ALIGN] +
			dirUpdate[i][ATTRACT]
		);
		// Update position here
		foid.pos += foid.dir * elapsedTime * 
			(this->speedVari ? glm::vec3(foid.speed) : glm::vec3(MAX_SPEED)) *
			this->speedFactor;
		// Relocate foid if it exceeds the window bounds
		if (glm::abs(foid.pos.x) >= 1.0)
		{
			foid.pos.x = (foid.pos.x / glm::abs(foid.pos.x)) * -0.99;
		}
		if (glm::abs(foid.pos.y) >= 1.0)
		{
			foid.pos.y = (foid.pos.y / glm::abs(foid.pos.y)) * -0.99;
		}
	}
	// Perform behavior
	calcBehavior(this->behavInfluence[AVOID],
		this->behavInfluence[ALIGN],
		this->behavInfluence[ATTRACT]);
}

void Flock::calcBehavior(float avoid, float align, float attract)
{
	// Calculate behavior for each flock member
	for (int i = 0; i < FLOCK_SIZE; i++)
	{
		Foid foid = this->population[i];
		// Initialize all new direction vectors and weights
		glm::vec3 newDir[3] = { glm::vec3(0, 0, 0) };
		float dirWeight[3] = { 0.0f };
		// Calculate behavior relative to all adjacent flock members
		for (int j = 0; j < FLOCK_SIZE; j++)
		{
			Foid adj = this->population[j];
			// Simulate partial field of view:
			// Calculate the angle 'alpha' between
			// flock member direction and adjacent members' position
			float adjDist = glm::distance(adj.pos, foid.pos);
			glm::vec3 adjDir = adj.pos - foid.pos;
			float alpha = glm::angle(foid.dir, adjDir);
			if (i != j &&
				alpha < (PI * this->awareVec[DEGREE]))
			{
				// Summate extremely close adjacent flock member data
				if(adjDist < this->awareVec[RADIUS] * 0.2)
				{
					newDir[AVOID] -= (adj.pos - foid.pos);
					dirWeight[AVOID] += 1.0;
				}
				// Summate moderately close adjacent flock member data
				else if(adjDist >= this->awareVec[RADIUS] * 0.4 &&
						adjDist <= this->awareVec[RADIUS] * 0.8)
				{
					newDir[ALIGN] += adj.dir;
					dirWeight[ALIGN] += glm::min(1.0, 0.0001 / adjDist);
				}
				// Summate peripheral adjacent flock member data
				else if(adjDist > this->awareVec[RADIUS] &&
						adjDist < this->awareVec[RADIUS] * 1.5)
				{
					newDir[ATTRACT] += adj.pos;
					dirWeight[ATTRACT] += 1.0;
				}
			}
		}
		// Calculate new direction vector for
		// avoiding adjacent flock members
		if (dirWeight[AVOID] > 0.0)
		{
			newDir[AVOID] = glm::normalize(newDir[AVOID] / dirWeight[AVOID]);
			dirUpdate[i][AVOID] = glm::vec3(avoid * 0.25) * newDir[AVOID];
		}
		// Calculate new direction vector for 
		// aligning with adjacent flock members
		if (dirWeight[ALIGN] > 0.0)
		{
			newDir[ALIGN] = glm::normalize(newDir[ALIGN] / dirWeight[ALIGN]);
			newDir[ALIGN] = newDir[ALIGN] - foid.dir;
			dirUpdate[i][ALIGN] = glm::vec3(align * 0.5) * newDir[ALIGN];
		}
		// Calculate new direction vector for 
		// attracting towards adjacent flock members
		if (dirWeight[ATTRACT] > 0.0)
		{
			// Normalized later since dealing w/ positions, not a direction
			newDir[ATTRACT] = newDir[ATTRACT] / dirWeight[ATTRACT];
			newDir[ATTRACT] = newDir[ATTRACT] - foid.pos;
			// Add some random variation to direction vector components
			newDir[ATTRACT] += glm::vec3(
				((((float)rand() / RAND_MAX) - 0.5) / 5),
				((((float)rand() / RAND_MAX) - 0.5) / 5),
				0);
			newDir[ATTRACT] = glm::normalize(newDir[ATTRACT]);
			dirUpdate[i][ATTRACT] = glm::vec3(attract * 0.2) *  newDir[ATTRACT];
		}
	}
}

glm::mat4 Flock::getModelMat(unsigned int index)
{
	Foid foid = this->population[index];
	// Pull position matrix values from foid's internal pos
	glm::mat4 Mpos = glm::translate(glm::mat4(1.0f), foid.pos);
	// Calculate rotation matrix using the foid's direction vector
	float alpha = -1.0 * (PI / 2.0);
	if (foid.dir.y != 0 && foid.dir.x != 0)
	{
		alpha += glm::atan(foid.dir.y, foid.dir.x);
	}
	glm::mat4 Mrot = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(0, 0, 1.0));
	// Multiply the position and rotation matrix to get the model matrix
	return (Mpos * Mrot);
}

void Flock::freeFlock()
{
	// Ignore for now, flock data isn't stored on heap
	/*for (int i = 0; i < FLOCK_SIZE; i++)
	{
		free(this->population[i]);
	}*/

}

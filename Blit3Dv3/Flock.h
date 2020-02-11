#pragma once
#include "Boid.h"

class Flock
{
public:
	//Variables
	std::vector<Boid*> boidList;

	//Constructor & Deconstructor
	Flock(int numberOfBoids);
	~Flock();

	//Methods
	void UpdateWorld(const double &seconds);
	void DrawWorld();
	void addBoid(float x, float y);
	void steeringBehaviors(const float &seconds);
	float distSquared(glm::vec2 posA, glm::vec2 posB);
	float distance(glm::vec2 posA, glm::vec2 posB);
	glm::vec2 align(Boid* current);
	glm::vec2 separation(Boid* current);
	glm::vec2 cohesion(Boid* current);
};

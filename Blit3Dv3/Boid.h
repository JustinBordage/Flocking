#pragma once
#include <Blit3D.h>

class Boid
{
public:
	//Variables
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 heading;

	//Constructor
	Boid(float positionX, float positionY);

	//Methods
	void Update(const double &seconds);
	void Draw();

	//Applies the steering force
	void applyForce(glm::vec2 force, const float &seconds);
};
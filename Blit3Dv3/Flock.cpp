#include "Flock.h"
#include "Random.h"

//Value Macros
#define SEPERATION_RADIUS 25.f
#define COHESION_RADIUS 50.f
#define MAXSPEED 200.f

//Radius Macros
#define INNER_RADIUS_SQR (SEPERATION_RADIUS * SEPERATION_RADIUS)
#define OUTER_RADIUS_SQR (COHESION_RADIUS * COHESION_RADIUS)

extern Random* rng;

//This is for debugging
void printVector(glm::vec2 vector, std::string varName)
{
	std::cout << varName << " X: " << vector.x << ", " << varName << " Y: " << vector.y << std::endl;
}

Flock::Flock(int numberOfBoids)
{
	//Generates boids to place on the screen
	for (int boids = 0; boids < numberOfBoids; boids++)
	{
		addBoid(rng->generateNum(0, 1920), rng->generateNum(0, 1080));
	}
}

Flock::~Flock()
{
	//Deletes all the boids that exist
	for (Boid* boid : boidList)
	{
		delete boid;
	}

	boidList.clear();
}

void Flock::UpdateWorld(const double &seconds)
{
	//Performs the steering calculations
	steeringBehaviors(seconds);

	//Updates each of the boids in the flock
	for (Boid* boid : boidList)
	{
		boid->Update(seconds);
	}
}

void Flock::DrawWorld()
{
	//Draws each of the boids in the flock
	for (Boid* boid : boidList)
	{
		boid->Draw();
	}
}

void Flock::addBoid(float x, float y)
{
	//Adds a Boid to the scene
	boidList.push_back(new Boid(x, y));
}

float Flock::distSquared(glm::vec2 posA, glm::vec2 posB)
{
	//Calculates the squared distance
	float distSqr = pow(posB.x - posA.x, 2) + pow(posB.y - posA.y, 2);

	//Returns the squared distance
	return distSqr;
}

float Flock::distance(glm::vec2 posA, glm::vec2 posB)
{
	//Calculates the squared distance
	float distSqr = distSquared(posA, posB);

	//Returns the distance
	return (distSqr > 0.f ? sqrt(distSqr) : 0.f);
}

void Flock::steeringBehaviors(const float &seconds)
{
	//Heading Force
	glm::vec2 headingForce = glm::vec2(0.f, 0.f);

	//Calculates a heading force for each boid
	for (Boid* boid : boidList)
	{
		headingForce = align(boid);
		headingForce += separation(boid) * 1.3f;
		headingForce += cohesion(boid) * 0.8f;

		boid->applyForce(glm::normalize(headingForce / seconds), seconds);
	}
}

//Uses velocity
glm::vec2 Flock::align(Boid* current)
{
	//Variables
	glm::vec2 avgVelocity = glm::vec2(0.f, 0.f);
	int counter = 0;
	float distSqr = 0.f;

	//Cycles through each boid in the flock
	for (Boid* other : boidList)
	{
		//Checks if 'other' is not itself
		if (other != current)
		{
			//Calculates the distance between the boids
			distSqr = distSquared(current->position, other->position);

			//Checks if we're within radius
			if (distSqr <= OUTER_RADIUS_SQR)
			{
				//Adds the velocity to the average
				avgVelocity += other->velocity;
				counter++;
			}
		}
	}

	if (counter > 0)
	{
		//Calculates the average based on how many boids were within radius
		avgVelocity = avgVelocity / (float)counter;

		//Removes the current velocity to determine the desired force
		avgVelocity = avgVelocity - current->velocity;

		//Returns the alignment force
		return avgVelocity;
	}

	//If no boids were in range it maintains the current velocity
	return current->velocity;
}

//Uses Position
glm::vec2 Flock::cohesion(Boid* current)
{
	//Variables
	glm::vec2 cohesionSteer = glm::vec2(0.f, 0.f);
	glm::vec2 avgPos = glm::vec2(0.f, 0.f);
	float distSqr = 0.f;
	int counter = 0;

	//Cycles through each boid in the flock
	for (Boid* other : boidList)
	{
		//Checks if 'other' is not itself
		if (other != current)
		{
			//Calculates the distance between the boids
			distSqr = distSquared(current->position, other->position);

			//Determines if the other boid is within alignment radius
			if (distSqr < OUTER_RADIUS_SQR)
			{
				//Adds the position to the average
				avgPos += other->position;
				counter++;
			}
		}
	}

	if (counter > 0)
	{
		//Calculates the average based on how many boids were within radius
		avgPos = avgPos / (float)counter;

		//Removes the current position to determine the desired force
		avgPos = avgPos - current->position;

		//Returns the Cohesion force
		return avgPos;
	}

	//If no boids were in range it maintains the current velocity
	return current->velocity;
}

//Uses Heading
glm::vec2 Flock::separation(Boid* current)
{
	glm::vec2 vector = glm::vec2(0.f, 0.f);
	glm::vec2 seperation = glm::vec2(0.f, 0.f);
	float distSqr = 0.f;
	int counter = 0;

	for (Boid* other : boidList)
	{
		if (other != current)
		{
			//Calculates the distance between the boids
			distSqr = distSquared(current->position, other->position);

			//Determines if the other boid is within seperation radius
			if (distSqr > 0.f && distSqr <= INNER_RADIUS_SQR)
			{
				//Determines a directional vector (with wrap around accounted for)
				vector = current->position - other->position;

				//Compiles the seperation steering vectors
				seperation += vector * (1 - (distSqr / INNER_RADIUS_SQR));
				counter++;
			}
		}
	}

	if (counter > 0)
	{
		//Calculates the average based on how many boids were within radius
		seperation /= (float)counter;

		//Multiplies the MaxSpeed to convert the heading into a velocity
		seperation *= MAXSPEED;

		//Removes the current velocity to determine the desired force
		seperation -= current->velocity;

		//Returns the Seperation Force
		return seperation;
	}

	//If no boids were in range it maintains the current velocity
	return current->velocity;
}
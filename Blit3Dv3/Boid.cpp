#include "Boid.h"
#include "Random.h"

//Externs
extern Sprite* BoidSprite;
extern Random* rng;

//Marcos
#define MAXSPEED 200.f
#define BOIDSCALE 0.3f
#define LERPFORCE 7.f

//Method Prototype
glm::vec2 basicLerp(glm::vec2 towards, glm::vec2 from, float speed);

Boid::Boid(float posX, float posY)
{
	//Sets the position
	position.x = posX;
	position.y = posY;

	//Generates an angle
	float radians = glm::radians(rng->generateNum(0, 360) * 1.f);

	//Calculates the initial heading
	heading.x = cos(radians);
	heading.y = sin(radians);
	
	//Calculates the initial speed
	velocity = heading * MAXSPEED;
}

void Boid::Update(const double &seconds)
{
	//Sets the speed of the velocity based on the heading
	velocity = heading * MAXSPEED;

	//Calculates the new position
	position += velocity * (float) seconds;

	//Wrap around Check
	if (position.x < 10.f) position.x += 1920.f;
	if (position.x > 1910.f) position.x -= 1920.f;
	if (position.y < 10.f) position.y += 1080.f;
	if (position.y > 1070.f) position.y -= 1080.f;
}

void Boid::Draw()
{
	//Determines the angle based on the heading
	BoidSprite->angle = glm::degrees(atan2(heading.y, heading.x)) - 90;

	//Original Sprite Drawing
	BoidSprite->Blit(position.x, position.y, BOIDSCALE, BOIDSCALE);

	//Edge Wrapping
	BoidSprite->Blit(position.x + 1920.f, position.y, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x - 1920.f, position.y, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x, position.y + 1080.f, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x, position.y - 1080.f, BOIDSCALE, BOIDSCALE);

	//Corner Wrapping
	BoidSprite->Blit(position.x + 1920.f, position.y + 1080.f, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x - 1920.f, position.y - 1080.f, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x - 1920.f, position.y + 1080.f, BOIDSCALE, BOIDSCALE);
	BoidSprite->Blit(position.x + 1920.f, position.y - 1080.f, BOIDSCALE, BOIDSCALE);
}

void Boid::applyForce(glm::vec2 newAccelerate, const float &seconds)
{
	//Adjusts the heading using linear interpolation
	heading += basicLerp(newAccelerate, heading, LERPFORCE * seconds);

	//Normalizes the adjusted heading
	heading = glm::normalize(heading);
}

glm::vec2 basicLerp(glm::vec2 towards, glm::vec2 from, float speed)
{
	//Basic linear interpolation calculation
	return (towards - from) * speed + from;
}
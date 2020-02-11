#include <Blit3D.h>

void angleCalculation()
{
	//make a vector
	glm::vec2 h(1, 1);

	//what angle is that vector pointing at?
	float angle = glm::degrees(atan2(h.y, h.x));

	glm::vec2 direction;

	//make vector direction point at angle
	direction.x = cos(glm::radians((angle)));
	direction.y = sin(glm::radians((angle)));
}

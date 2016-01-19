#pragma once

#include "glm/glm.hpp"

class Ray{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	static Ray ShootRay(int i, int j);

};
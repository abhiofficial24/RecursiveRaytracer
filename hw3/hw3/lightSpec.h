#pragma once

#include "glm/glm.hpp"

//simple class to hold details about lights
class lightSpec{
public:
	enum lightType{
		Directional = 0,
		Point
	};

	lightType type;
	glm::vec3 position;
	glm::vec3 color;

};
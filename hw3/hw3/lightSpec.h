#pragma once

#include "glm/glm.hpp"

//simple class to hold details about lights
class lightSpec{
public:

	lightSpec();

	enum lightType{
		Directional = 0,
		Point
	};

	//Returns the distance in world space units from a position this light source is.
	//If a geometric object blocks the path it returns -1.0
	float IsVisible(glm::vec3 visiblePos);

	lightType type;
	glm::vec3 position;
	glm::vec3 color;

};
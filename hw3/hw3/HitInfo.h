#pragma once

#include "ray.h"

class GeometryObject;

//Data for when an intersection occurs
class HitInfo{
public:
	//The ray that caused the intersection
	Ray raycast;
	float t;

	//Where the intersected occured
	glm::vec3 position;

	//Object that was intersected with
	GeometryObject* collisionObject;

	//Normal from intersection
	glm::vec3 normal;
};

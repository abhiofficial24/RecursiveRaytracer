#pragma once

#include <iostream>
#include "glm/glm.hpp"

#include "HitInfo.h"
#include "ray.h"

class GeometryObject{
public:
	virtual HitInfo Intersect(float*& colorVals, Ray ray){ return HitInfo(); };
	glm::mat4 transform;
	
	Ray TransformRay(Ray ray);

};

class Quad : public GeometryObject{
public:
	Quad();
	Quad(glm::vec3 inputPoints[4]);

	//Define how intersection will return a color.
	HitInfo Intersect(float*& colorVals, Ray ray) override;

	glm::vec3 points[4];
};

class Triangle : public GeometryObject{
public:
	Triangle();
	Triangle(glm::vec3 inputPoints[3]);

	//Define how the intersection will return a color
	HitInfo Intersect(float*& colorVals, Ray ray) override;

	//Points that define the triangle
	glm::vec3 points[3];
};

class Sphere : public GeometryObject{
public:
	Sphere();
	//Specify position and radius
	Sphere(glm::vec3 pos, float rad);

	//Define how the intersection will return a color
	HitInfo Intersect(float*& colorVals, Ray ray) override;

	glm::vec3 position;
	float radius;

};
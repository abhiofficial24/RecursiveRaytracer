#pragma once

#include <iostream>
#include "glm/glm.hpp"

#include "HitInfo.h"
#include "ray.h"

class GeometryObject{
public:
	virtual HitInfo Intersect(Ray ray){ return HitInfo(); };
	glm::mat4 transform;
	
	//All of the material properties
	glm::vec3 diffuse;
	float shininess;
	glm::vec3 specular;
	glm::vec3 emission;

	glm::vec3 ambient;

	Ray TransformRay(Ray ray);

	glm::vec3 InverseTransformNormal(glm::vec3 normal);

};

class Quad : public GeometryObject{
public:
	Quad();
	Quad(glm::vec3 inputPoints[4]);

	//Define how intersection will return a color.
	HitInfo Intersect(Ray ray) override;

	glm::vec3 points[4];
};

class Triangle : public GeometryObject{
public:
	Triangle();
	Triangle(glm::vec3 inputPoints[3]);

	//Define how the intersection will return a color
	HitInfo Intersect(Ray ray) override;

	//Points that define the triangle
	glm::vec3 points[3];
};

class Sphere : public GeometryObject{
public:
	Sphere();
	//Specify position and radius
	Sphere(glm::vec3 pos, float rad);

	//Define how the intersection will return a color
	HitInfo Intersect(Ray ray) override;

	glm::vec3 position;
	float radius;

};
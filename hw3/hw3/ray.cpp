#include "ray.h"
#include "transform.h"
#include "readfile.h"

#include <iostream>

Ray Ray::ShootRay(int i, int j){

	//We need the coordinate frame vectors u, v, and w
	glm::vec3 u = Transform::u;
	glm::vec3 v = Transform::v;
	glm::vec3 w = Transform::w;

	//Cast to floats to avoid innacuracies
	float fFOVY = (float)fovy;
	float fJ = (float)j;
	float fI = (float)i;
	float fWIDTH = (float)WIDTH;
	float fHEIGHT = (float)HEIGHT;

	//I also need the x field of view
	float fovx = 2 * atan(tan(fFOVY / 2)*(fWIDTH / fHEIGHT));

	//I think I can just do the equations for alpha and beta without 
	//deriving them now
	float alpha = (tan(fovx / 2)) * ((fI - (fWIDTH / 2)) / (fWIDTH / 2));
	float beta = (tan(fFOVY / 2)) * (((fHEIGHT / 2) - fJ) / (fHEIGHT / 2));

	//Alpha and beta now allow us to get the direction vector.
	//That along with the ray center which is just the camera center gives us the ray to return
	glm::vec3 rayDir = glm::normalize((alpha)*u + (beta)*v - w);

	//Create the ray to return now that we have center and direction.
	Ray returnRay;
	returnRay.origin = eyeinit;
	returnRay.direction = rayDir;

	return returnRay;
}
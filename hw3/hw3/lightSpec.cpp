#include "lightSpec.h"
#include "ray.h"
#include "Utilities.h"

lightSpec::lightSpec(){
	type = lightSpec::Point;
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(1.0, 1.0, 1.0);
}

float lightSpec::IsVisible(glm::vec3 visiblePos){

	//Set up the ray from our origin to the light
	Ray ray = Ray();
	ray.origin = position;
	ray.direction = glm::normalize(visiblePos - position);
	float possibleDistance = sqrt(pow(position.x - visiblePos.x, 2.0) + pow(position.y - visiblePos.y, 2.0) + pow(position.z - visiblePos.z, 2.0));
	//Now try to intersect every object
	HitInfo info = Utilities::RayIntersect(ray);

	float threshold = 0.001;

	//If nothing was intersected before the proposed distance from the light position to the visiblePos
	if (info.t >= possibleDistance - threshold){
		//THen the possibleDistance is the actual distance
		return possibleDistance;
	}
	//This indicates the light is not visible
	return -1.0;
}
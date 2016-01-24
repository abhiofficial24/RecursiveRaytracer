#define EXPORT_TEST_FUNCTIONS

#include "Utilities.h"
#include "objDefinitions.h"
#include "vector"
#include "readfile.h"

HitInfo Utilities::RayIntersect(Ray ray){
	bool writeTo = false;
	//Create a HitInfo, we will test it on every geometry object we can
	HitInfo hitInfo = HitInfo();
	//Set a really high distance threshold
	hitInfo.t = 100;
	for (std::vector<GeometryObject*>::iterator it = geometryVector.begin(); it != geometryVector.end(); ++it){
		//allocate a pointer for storing the color data
		if (dynamic_cast<Quad*>(*it) != nullptr){
			Quad* quad = dynamic_cast<Quad*>(*it);
			//Get the new color values
			hitInfo = quad->Intersect(ray);
			writeTo = true;
		}
		else if (dynamic_cast<Triangle*>(*it) != nullptr){
			Triangle* triangle = dynamic_cast<Triangle*>(*it);
			//Get the new color values
			HitInfo curHitInfo = triangle->Intersect(ray);
			//If the new intersection is closer, it is the new hit info
			if (curHitInfo.t < hitInfo.t){
				hitInfo = curHitInfo;
			}
		}
		else if (dynamic_cast<Sphere*>(*it) != nullptr){
			Sphere* sphere = dynamic_cast<Sphere*>(*it);
			//Get the new color values
			HitInfo curHitInfo = sphere->Intersect(ray);
			//If the new intersection is closer, it is the new hit info
			if (curHitInfo.t < hitInfo.t){
				hitInfo = curHitInfo;
			}
		}
	}
	return hitInfo;

}


void Utilities::FindColor(HitInfo info, float* colorVals){

	//Initialize to blue
	colorVals[0] = 0.0;
	colorVals[1] = 0.0;
	colorVals[2] = 1.0;

	//If the collision actually occured
	if (info.t < 100){

		//A hit was found so add ambient and emission
		colorVals[0] = ambient[0] + info.collisionObject->emission[0];
		colorVals[1] = ambient[1] + info.collisionObject->emission[1];
		colorVals[2] = ambient[2] + info.collisionObject->emission[2];

		//Iterate through each light
		for (int i = 0; i < numused; i++){

			lightSpec currentLight = lightData[i];

			//Transform the position of the position by it's objects transform
			glm::vec4 transformedPos4 = info.collisionObject->transform * glm::vec4(currentLight.position.x, currentLight.position.y, currentLight.position.z, 1);

			transformedPos4 /= transformedPos4.w;

			glm::vec3 transformedPos3 = glm::vec3(transformedPos4.x, transformedPos4.y, transformedPos4.z);

			float distToLight = currentLight.IsVisible(transformedPos3);

			if (distToLight != -1.0){

				//Transform normal
				glm::vec3 normal = info.normal;

				//Now do the real lighting calculations
				glm::vec3 direction = transformedPos3 - info.position;
				direction = glm::normalize(direction);

				glm::vec3 eyedirn = eyeinit - info.position;
				eyedirn = glm::normalize(eyedirn);

				glm::vec3 half = direction + eyedirn;
				half = glm::normalize(half);

				glm::vec3 newColor;
				float term1 = (1 / (attenuation[0] + (attenuation[1] * distToLight) + (attenuation[2] * pow(distToLight, 2))));
				glm::vec3 term2 = info.collisionObject->diffuse;//*std::fmax(glm::dot(info.normal, direction), 0));
				term2[0] *= std::fmax(glm::dot(info.normal, direction), 0);
				term2[1] *= std::fmax(glm::dot(info.normal, direction), 0);
				term2[2] *= std::fmax(glm::dot(info.normal, direction), 0);
				glm::vec3 term3 = (info.collisionObject->specular);

				term3[0] *= pow(std::fmax(glm::dot(info.normal, half), 0), 2);
				term3[1] *= pow(std::fmax(glm::dot(info.normal, half), 0), 2);
				term3[2] *= pow(std::fmax(glm::dot(info.normal, half), 0), 2);

				glm::vec3 combinedTerms = term2 + term3;
				newColor[0] = combinedTerms[0] * term1;
				newColor[1] = combinedTerms[1] * term1;
				newColor[2] = combinedTerms[2] * term1;


				colorVals[0] += newColor[0] * 255.0;
				colorVals[1] += newColor[1] * 255.0;
				colorVals[2] += newColor[2] * 255.0; 
			}
		}
	}
}
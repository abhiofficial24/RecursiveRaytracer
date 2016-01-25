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


void Utilities::FindColor(HitInfo info, float* colorVals, int recDepth){

	//Initialize to blue
	colorVals[0] = 0.0;
	colorVals[1] = 0.0;
	colorVals[2] = 0.0;

	if (recDepth > 5){
		return;
	}

	//If the collision actually occured
	if (info.t < 100){

		//A hit was found so add ambient and emission
		colorVals[0] = info.collisionObject->ambient[0] + info.collisionObject->emission[0];
		colorVals[1] = info.collisionObject->ambient[1] + info.collisionObject->emission[1];
		colorVals[2] = info.collisionObject->ambient[2] + info.collisionObject->emission[2];

		//Iterate through each light
		for (int i = 0; i < numused; i++){

			lightSpec currentLight = lightData[i];

			//Transform the position of the position by it's objects transform
			glm::vec4 transformedPos4;
			glm::vec3 transformedPos3;

			if (currentLight.type == lightSpec::Point){
				transformedPos4 = info.collisionObject->transform * glm::vec4(currentLight.position.x, currentLight.position.y, currentLight.position.z, 1);

				//glm::vec4 transformedPos4 = info.collisionObject->transform * glm::vec4(info.position.x, info.position.y, info.position.z, 1);

				transformedPos4 /= transformedPos4.w;
			}
			//Directional lights have a w of 0 and do not need to be dehomogenized.
			else{
				transformedPos4 = info.collisionObject->transform * glm::vec4(currentLight.position.x, currentLight.position.y, currentLight.position.z, 0);
			}

			transformedPos3 = glm::vec3(transformedPos4.x, transformedPos4.y, transformedPos4.z);

			float distToLight = currentLight.IsVisible(transformedPos3);

			if (distToLight != -1.0){
				
				//Transform normal
				glm::vec3 normal = info.normal;

				//Now do the real lighting calculations
				glm::vec3 direction;

				//Point light direction
				if (currentLight.type == lightSpec::Point){
					direction = transformedPos3 - info.position;
					direction = glm::normalize(direction);
				}
				//Directional light direction is just it's position.
				else{
					direction = transformedPos3;
					direction = glm::normalize(direction);
				}

				glm::vec3 eyedirn = eyeinit - info.position;
				eyedirn = glm::normalize(eyedirn);

				glm::vec3 half = direction + eyedirn;
				half = glm::normalize(half);

				glm::vec3 newColor;
				float term1R = (currentLight.color[0] / (attenuation[0] + (attenuation[1] * distToLight) + (attenuation[2] * pow(distToLight, 2))));
				float term1G = (currentLight.color[1] / (attenuation[0] + (attenuation[1] * distToLight) + (attenuation[2] * pow(distToLight, 2))));
				float term1B = (currentLight.color[2] / (attenuation[0] + (attenuation[1] * distToLight) + (attenuation[2] * pow(distToLight, 2))));
				glm::vec3 term2 = info.collisionObject->diffuse;
				term2[0] *= std::fmax(glm::dot(normal, direction), 0);
				term2[1] *= std::fmax(glm::dot(normal, direction), 0);
				term2[2] *= std::fmax(glm::dot(normal, direction), 0);
				glm::vec3 term3 = (info.collisionObject->specular);

				term3[0] *= pow(std::fmax(glm::dot(normal, half), 0), specular[0]);
				term3[1] *= pow(std::fmax(glm::dot(normal, half), 0), specular[1]);
				term3[2] *= pow(std::fmax(glm::dot(normal, half), 0), specular[2]);

				glm::vec3 combinedTerms = term2 + term3;
				newColor[0] = combinedTerms[0] * term1R;
				newColor[1] = combinedTerms[1] * term1G;
				newColor[2] = combinedTerms[2] * term1B;
				

				//Do reflection if necessary
				glm::vec3 reflectedRayDirection = 2 * (glm::dot(info.normal, info.raycast.direction))*(info.normal - info.raycast.direction);
				glm::vec3 reflectedRayOrigin = info.position;

				Ray reflectedRay = Ray();
				reflectedRay.direction = reflectedRayDirection;
				reflectedRay.origin = reflectedRayOrigin;

				//Intersect with this reflected ray.
				HitInfo reflectionInfo = RayIntersect(reflectedRay);

				float* reflectionColor = new float[3];
				reflectionColor[0] = 0;
				reflectionColor[1] = 0;
				reflectionColor[2] = 0;

				if (info.collisionObject->specular[0] != 0 ||
					info.collisionObject->specular[1] != 0||
					info.collisionObject->specular[2] != 0){
					//Make a reflection color and increment the reflection index
					float* reflectionColor = new float[3];
					recDepth++;
					//recursive call to find color
					FindColor(reflectionInfo, reflectionColor, recDepth);
				}


				colorVals[0] += newColor[0] + info.collisionObject->shininess * reflectionColor[0];
				colorVals[1] += newColor[1] + info.collisionObject->shininess * reflectionColor[1];
				colorVals[2] += newColor[2] + info.collisionObject->shininess * reflectionColor[2];
			}
		}
	}
}
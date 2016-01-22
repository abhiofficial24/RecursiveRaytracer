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
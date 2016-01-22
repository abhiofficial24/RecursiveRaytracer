#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "glm/glm.hpp"

// Main variables in the program.  
#define MAINPROGRAM 
#include "readfile.h"
#include "ray.h"
#include "objDefinitions.h"
#include "transform.h"
#include <FreeImage.h>
#include "HitInfo.h"
#include "Utilities.h"

#define BPP 24 // 8 bits per color and 3 colors per pixel

using namespace std;

HitInfo RayIntersect(Ray ray, int count){
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

void FindColor(HitInfo info, float* colorVals){

	//Intialize to all black
	colorVals[0] = 0.0;
	colorVals[1] = 0.0;
	colorVals[2] = 0.0;

	//If the collision actually occured
	if (info.t < 100){
		//Iterate through each light
		for (int i = 0; i < numused; i++){

			lightSpec currentLight = lightData[i];
			


			//Transform the position of the position by it's objects transform
			glm::vec4 transformedPos4 = info.collisionObject->transform * glm::vec4(info.position.x, info.position.y, info.position.z, 1);

			transformedPos4 /= transformedPos4.w;

			glm::vec3 transformedPos3 = glm::vec3(transformedPos4.x, transformedPos4.y, transformedPos4.z);

			float distToLight = currentLight.IsVisible(transformedPos3);

			if (distToLight != -1.0){
				colorVals[0] += currentLight.color.r * (255.0/numLights);
				colorVals[1] += currentLight.color.b * (255.0/numLights);
				colorVals[2] += currentLight.color.g * (255.0/numLights);
			}
		}
	}
}

float* Raytrace(){
	//Open a log file
	ofstream myfile;
	myfile.open("log.txt");
	int count = 0;
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			Ray nextRay = Ray::ShootRay(i, j);
			HitInfo hitInfo = Utilities::RayIntersect(nextRay);
			float* colorVals = new float[3];
			FindColor(hitInfo, colorVals);
			ColorData[count] = colorVals[0];
			ColorData[count + 1] = colorVals[1];
			ColorData[count + 2] = colorVals[2];
			count += 3;
		}
	}
	//close the file
	myfile.close();
	return ColorData;
}



void Create_Image(float* ColorData, int width, int height, std::string imageName){
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, BPP);
	RGBQUAD color;
	int count = 0;
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			color.rgbRed = ColorData[count];
			count++;
			color.rgbGreen = ColorData[count];
			count++;
			color.rgbBlue = ColorData[count];
			count++;
			//Now assign the new colors if a hit occured
			
			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	if (FreeImage_Save(FIF_PNG, bitmap, imageName.c_str(), 0))
		cout << "Image_successfully_saved!" << endl;
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}

	FreeImage_Initialise();

	readfile(argv[1]);

	//Make the coordinate frame now for later use
	Transform::MakeCoordFrame(eyeinit, center, up);

	FIBITMAP* bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);

	if (!bitmap){
		//Image could not be allocated
		exit(1);
	}

	ColorData = Raytrace();

	Create_Image(ColorData, WIDTH, HEIGHT, "test.png");

	FreeImage_DeInitialise();

	//Keep the window alive
	getchar();
}
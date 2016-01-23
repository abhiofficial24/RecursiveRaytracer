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
			Utilities::FindColor(hitInfo, colorVals);
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
			color.rgbRed = ColorData[count] * 255;
			count++;
			color.rgbGreen = ColorData[count] * 255;
			count++;
			color.rgbBlue = ColorData[count] * 255;
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
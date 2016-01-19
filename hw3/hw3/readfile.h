#pragma once

#include <stack>
#include <vector>
#include "objDefinitions.h"
#include "lightSpec.h"

//Temp variable definitions

//Read in to create the height and width of the image
extern int HEIGHT;
extern int WIDTH;

extern glm::vec3 center;
extern glm::vec3 eyeinit;
extern glm::vec3 upinit;
extern glm::vec3 up;
extern float fovy;
extern std::vector<glm::vec3> vertexList;
extern float* ColorData;
extern std::vector<GeometryObject*> geometryVector;

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10;
extern lightSpec lightData[numLights]; // Light Specification data
extern int numused;

// Readfile definitions 
bool readvals(std::stringstream &s, const int numvals, float * values);
void readfile(const char * filename);

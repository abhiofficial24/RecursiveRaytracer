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
extern int maxverts;
extern float* ColorData;
extern std::vector<GeometryObject*> geometryVector;

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10;
extern lightSpec lightData[numLights]; // Light Specification data
extern int numused;

//other lighting properties
extern glm::vec3 attenuation;
extern glm::vec3 ambient;

// Readfile definitions 
bool readvals(std::stringstream &s, const int numvals, float * values);
void readfile(const char * filename);

//Current material properties
extern glm::vec3 diffuse;
extern glm::vec3 specular;
extern glm::vec3 emission;
extern float shininess;

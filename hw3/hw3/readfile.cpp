#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <vector>
#include "glm/glm.hpp"

#include "transform.h"
#include "readfile.h"
#include "objDefinitions.h"

#define BPP 24 // 8 bits per color and 3 colors per pixel
int HEIGHT;
int WIDTH;

glm::vec3 center;
glm::vec3 eyeinit;
glm::vec3 upinit;
glm::vec3 up;
float fovy;
std::vector<glm::vec3> vertexList;
float* ColorData;
std::vector<GeometryObject*> geometryVector;


// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(std::stack<glm::mat4> &transfstack, float* values)
{
	glm::mat4 transform = transfstack.top();
	glm::vec4 valvec = glm::vec4(values[0], values[1], values[2], values[3]);
	glm::vec4 newval = transform * valvec;
	for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const glm::mat4 & M, std::stack<glm::mat4> &transfstack)
{
	glm::mat4 &T = transfstack.top();
	T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(std::stringstream &s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			std::cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}



void readfile(const char* filename){
	std::string str, cmd;
	std::ifstream in;
	in.open(filename);
	if (in.fail()){
		std::cout << "File opening has failed" << std::endl;
	}
	if (in.is_open()) {

		// I need to implement a matrix stack to store transforms.  
		// This is done using standard STL Templates 
		std::stack <glm::mat4> transfstack;
		transfstack.push(glm::mat4(1.0));  // identity

		getline(in, str);
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
				// Ruled out comment and blank lines 

				std::stringstream s(str);
				s >> cmd;
				//int i;
				float values[10]; // Position and color for light, colors for others
				// Up to 10 params for cameras.  
				bool validinput; // Validity of input 

				//Dimension values
				if (cmd == "size"){
					validinput = readvals(s, 2, values); // 10 values eye cen up fov
					if (validinput){

						//width of the image
						WIDTH = (int)values[0];

						//height of the image
						HEIGHT = (int)values[1];

						std::cout << "Width of image is " << WIDTH << " and height of image is " << HEIGHT << "." << std::endl;
						ColorData = new float[WIDTH*HEIGHT*3];
						//Initialize all color data to 0
						for (int i = 0; i < WIDTH*HEIGHT*3; i++){
							ColorData[i] = 0.0;
						}
					}
				}
				//Process camera values
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if (validinput) {

						// Use all of values[0...9]
						// You may need to use the upvector fn in Transform.cpp
						// to set up correctly. 
						// Set eyeinit upinit center fovy in variables.h

						center[0] = values[3];
						center[1] = values[4];
						center[2] = values[5];

						eyeinit[0] = values[0];
						eyeinit[1] = values[1];
						eyeinit[2] = values[2];


						up = glm::vec3(values[6], values[7], values[8]);
						glm::vec3 zVec = eyeinit - center;
						zVec = glm::normalize(zVec);

						upinit = Transform::upvector(up, zVec);
						up = upinit;

						fovy = values[9];
						std::cout << "up values( " << up[0] << ", " << up[1] << ", " << up[2] << ")" << std::endl;
						std::cout << "center values( " << center[0] << ", " << center[1] << ", " << center[2] << ")" << std::endl;
						std::cout << "eyeinit values( " << eyeinit[0] << ", " << eyeinit[1] << ", " << eyeinit[2] << ")" << std::endl;
						std::cout << "fov value " << values[9] << std::endl;


					}
				}
				else if (cmd == "vertex"){
					validinput = readvals(s, 3, values); // 4 values for each point of the quad
					if (validinput){
						glm::vec3 vertex = glm::vec3(values[0], values[1], values[2]);
						//Now push this onto the stack
						vertexList.push_back(vertex);
						//Debug values
						std::cout << "Vertex value of (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") " << std::endl;
					}
				}
				else if (cmd == "quad"){
					validinput = readvals(s, 4, values); // 4 values for each point of the quad
					if (validinput){
						glm::vec3 points[4];
						//Assign all 
						for (int i = 0; i < 4; i++){
							if (vertexList.size() >= (int)values[i]){
								points[i] = vertexList[values[i] - 1]; //-1 corrects for 0 indexing. Input file specifies first vertex as 1, not 0.
							}
							//If there are not vertices on the stack then 
							else{
								std::cout << "No vertices on the stack" << std::endl;
								points[i] = glm::vec3(0.0);
							}
						}

						//Just create the point for now
						Quad* curQuad = new Quad(points);

						/*
						std::cout << "Quad vertex 1 value of (" << &points[0][0] << ", " << &points[0][1] << ", " << &points[0][2] << ") " << std::endl;
						std::cout << "Quad vertex 2 value of (" << &points[1][0] << ", " << &points[1][1] << ", " << &points[1][2] << ") " << std::endl;
						std::cout << "Quad vertex 3 value of (" << &points[2][0] << ", " << &points[2][1] << ", " << &points[2][2] << ") " << std::endl;
						std::cout << "Quad vertex 4 value of (" << &points[3][0] << ", " << &points[3][1] << ", " << &points[3][2] << ") " << std::endl;
						*/

						//Give it the current transform
						curQuad->transform = transfstack.top();

						//Push it onto the stack.
						geometryVector.push_back(curQuad);
					}
				}
				else if (cmd == "tri"){
					validinput = readvals(s, 3, values); // 4 values for each point of the quad
					if (validinput){
						glm::vec3 points[3];
						//Assign all 
						for (int i = 0; i < 3; i++){
							if (vertexList.size() >= (int)values[i]){
								points[i] = vertexList[values[i]];
							}
							//If there are not vertices on the stack then 
							else{
								std::cout << "No vertices on the stack" << std::endl;
								points[i] = glm::vec3(0.0);
							}
						}

						//Just create the point for now
						Triangle* curTriangle = new Triangle(points);
						
						//Give it the current transform
						curTriangle->transform = transfstack.top();

						//Push it onto the stack.
						geometryVector.push_back(curTriangle);
					}
				}
				else if (cmd == "sphere"){
					validinput = readvals(s, 4, values); // 4 values, the x,y, z poisition and the radius
					if (validinput){
						glm::vec3 position;
						//Get the readius
						float radius = values[3];

						//Get the position
						position[0] = values[0];
						position[1] = values[1];
						position[2] = values[2];

						//Create the sphere
						Sphere* curSphere = new Sphere(position, radius);

						//Give it the current transform
						curSphere->transform = transfstack.top();

						//push the sphere onto the stack
						geometryVector.push_back(curSphere);

					}
				}
				else if (cmd == "translate"){
					validinput = readvals(s, 3, values);
					if (validinput) {
						const glm::mat4 transMat = Transform::translate(values[0], values[1], values[2]);
						rightmultiply(transMat, transfstack);
					}
				}
				else if (cmd == "rotate"){
					validinput = readvals(s, 4, values);
					if (validinput){
						//Get the axis of rotation
						const glm::vec3 axis = glm::vec3(values[0], values[1], values[2]);
						glm::vec3 normAxis = glm::normalize(axis);
						//Generate the 3x3 rotation matrix
						glm::mat3 rotMat3 = Transform::rotate(values[3], -normAxis);
						glm::mat4 rotMat4 = glm::mat4(1.0);

						//Create 4x4 rotation matrix to use
						rotMat4[0][0] = rotMat3[0][0]; rotMat4[1][0] = rotMat3[1][0]; rotMat4[2][0] = rotMat3[2][0]; rotMat4[3][0] = 0;
						rotMat4[0][1] = rotMat3[0][1]; rotMat4[1][1] = rotMat3[1][1]; rotMat4[2][1] = rotMat3[2][1]; rotMat4[3][1] = 0;
						rotMat4[0][2] = rotMat3[0][2]; rotMat4[1][2] = rotMat3[1][2]; rotMat4[2][2] = rotMat3[2][2]; rotMat4[3][2] = 0;
						rotMat4[0][3] = 0;			   rotMat4[1][3] = 0;			  rotMat4[2][3] = 0;			 rotMat4[3][3] = 1;

						rightmultiply(rotMat4, transfstack);
					}
				}
				else if (cmd == "scale"){
					validinput = readvals(s, 3, values);
					if (validinput) {
						const glm::mat4 scaleMat = Transform::scale(values[0], values[1], values[2]);
						rightmultiply(scaleMat, transfstack);
					}
				}
				else if (cmd == "pushTransform"){
					transfstack.push(transfstack.top());
				}
				else if (cmd == "popTransform"){
					if (transfstack.size() <= 1){
						std::cerr << "Stack has no elements.  Cannot Pop\n";
					}
					else{
						transfstack.pop();
					}
				}
				//Not recognized command
				else{
					std::cout << "command " << cmd << " is not recognized" << std::endl;
				}
			}
			getline(in, str);
		}
	}
}
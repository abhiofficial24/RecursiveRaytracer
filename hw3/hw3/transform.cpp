#include "transform.h"


glm::vec3 Transform::u;
glm::vec3 Transform::v;
glm::vec3 Transform::w;

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

glm::vec3 Transform::upvector(const glm::vec3 &up, const glm::vec3 & zvec)
{
	glm::vec3 x = glm::cross(up, zvec);
	glm::vec3 y = glm::cross(zvec, x);
	glm::vec3 ret = glm::normalize(y);
	return ret;
}


// Helper rotation function.  Please implement this.  
glm::mat3 Transform::rotate(const float degrees, const glm::vec3& axis)
{

	// YOUR CODE FOR HW1 HERE
	// normalize axis if you haven't already
	glm::vec3 normAxis = glm::normalize(axis);
	// Convert pi to radians
	const float piToRad = pi / 180;
	// Declare an array to return 
	glm::mat3 returnMatrix = glm::mat3();
	//We need the identity matrix
	glm::mat3 identity = glm::mat3(1.0);

	float cosAngle = cos(degrees * piToRad);
	float sinAngle = sin(degrees * piToRad);

	/* axis times its transpose to make that matrix
	* | axis.x*axis.x   axis.x*axis.y   axis.x*axis.z  |
	* | axis.y*axis.x   axis.y*axis.y   axis.y*axis.y  |
	* | axis.z*axis.x   axis.z*axis.y   axis.z*axis.z  |
	*/

	glm::mat3 aaT = glm::mat3();
	aaT[0][0] = normAxis.x*normAxis.x;  aaT[0][1] = normAxis.x*normAxis.y;  aaT[0][2] = normAxis.x*normAxis.z;
	aaT[1][0] = normAxis.y* normAxis.x; aaT[1][1] = normAxis.y*normAxis.y;  aaT[1][2] = normAxis.z*normAxis.y;
	aaT[2][0] = normAxis.z*normAxis.x;  aaT[2][1] = normAxis.z*normAxis.y;  aaT[2][2] = normAxis.z*normAxis.z;

	glm::mat3 dualMatrix = glm::mat3();

	/* dual matrix of axis
	* |     0       -normAxis.z  normAxis.y |
	* | normAxis.z      0       -normAxis.x |
	* | -normAxis.y  normAxis.x       0     |
	*/

	dualMatrix[0][0] = 0; dualMatrix[0][1] = -normAxis.z; dualMatrix[0][2] = normAxis.y;
	dualMatrix[1][0] = normAxis.z; dualMatrix[1][1] = 0; dualMatrix[1][2] = -normAxis.x;
	dualMatrix[2][0] = -normAxis.y; dualMatrix[2][1] = normAxis.x; dualMatrix[2][2] = 0;

	returnMatrix = identity*cosAngle + aaT*(1 - cosAngle) + dualMatrix*sinAngle;

	// You will change this return call
	return returnMatrix;

}


void Transform::left(float degrees, glm::vec3& eye, glm::vec3& up)
{
	//eye = glm::normalize(eye);
	up = glm::normalize(up);

	glm::mat3 rotMatrix = rotate(-degrees, up);
	eye = rotMatrix*eye;
	up = rotMatrix*up;
	up = glm::normalize(up);
}


void Transform::up(float degrees, glm::vec3& eye, glm::vec3& up)
{
	//eye = glm::normalize(eye);
	up = glm::normalize(up);

	glm::vec3 right = glm::cross(eye, up);
	right = glm::normalize(right);
	glm::mat3 rotMatrix = rotate(-degrees, right);
	eye = rotMatrix*eye;
	up = rotMatrix*up;
	up = glm::normalize(up);
}


void Transform::MakeCoordFrame(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up){
	//Create coordinate plane
	glm::vec3 a = eye - center;
	glm::vec3 b = up;

	//Find w, then u, and then v, of the uvw coordinate plane
	float aMag = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	//w is static member of this class
	w = a / aMag;

	glm::vec3 nonUnitU = glm::cross(b, w);
	float uMag = sqrt(pow(nonUnitU.x, 2) + pow(nonUnitU.y, 2) + pow(nonUnitU.z, 2));

	//u is static member of this class
	u = nonUnitU / uMag;

	//v is static member of this class
	v = glm::cross(w, u);
}

glm::mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	glm::mat4 ret;
	// YOUR CODE FOR HW2 HERE
	// Implement scaling 
	ret = glm::mat4(1.0);
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
	return ret;
}

glm::mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	glm::mat4 ret;
	// YOUR CODE FOR HW2 HERE
	// Implement translation 
	ret = glm::mat4(1.0);
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;
	return ret;
}
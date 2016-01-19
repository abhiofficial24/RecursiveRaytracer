#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float pi = 3.14159265; // For portability across platforms

class Transform
{
public:
	Transform();
	virtual ~Transform();
	static glm::vec3 upvector(const glm::vec3 &up, const glm::vec3 &zvec);
	//Construct the coordinate frame from the propery eye location, center view vector, and camera up vector
	static void MakeCoordFrame(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
	static glm::mat3 rotate(const float degrees, const glm::vec3& axis);
	static glm::mat4 scale(const float &sx, const float &sy, const float &sz);
	static glm::mat4 translate(const float &tx, const float &ty, const float &tz);
	static void left(float degrees, glm::vec3& eye, glm::vec3& up);
	static void up(float degrees, glm::vec3& eye, glm::vec3& up);

	//The u,v, and w coordinate frame vectors that should never change.
	static glm::vec3 u;
	static glm::vec3 v;
	static glm::vec3 w;
};
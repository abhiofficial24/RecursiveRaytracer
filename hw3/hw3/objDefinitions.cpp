
#include "objDefinitions.h"
#include "HitInfo.h"


//return a ray transformed by the inverse of the transform;
Ray GeometryObject::TransformRay(Ray ray){
	//Get the inverse of the transform
	glm::mat4 inverseMat = glm::inverse(transform);

	//Now transform both the origin and direction.
	//origin has w=1 but direction is a direction vector, therefore has w=0;

	//Origin
	glm::vec4 transformedOrigin;
	transformedOrigin[0] = ray.origin[0];
	transformedOrigin[1] = ray.origin[1];
	transformedOrigin[2] = ray.origin[2];
	transformedOrigin[3] = 1;

	//Transform origin
	transformedOrigin = inverseMat*transformedOrigin;

	//Direction
	glm::vec4 transformedDirection;
	transformedDirection[0] = ray.direction[0];
	transformedDirection[1] = ray.direction[1];
	transformedDirection[2] = ray.direction[2];
	transformedDirection[3] = 0;

	//Transform direction
	transformedDirection = inverseMat*transformedDirection;

	//Create ray and set direction and origin
	Ray returnRay = Ray();
	returnRay.direction[0] = transformedDirection[0];
	returnRay.direction[1] = transformedDirection[1];
	returnRay.direction[2] = transformedDirection[2];

	returnRay.origin[0] = transformedOrigin[0];
	returnRay.origin[1] = transformedOrigin[1];
	returnRay.origin[2] = transformedOrigin[2];

	//Homongenize origin coordinates
	//Not done for direction because w is 0
	returnRay.origin /= transformedOrigin[3];

	return returnRay;

}


glm::vec3 GeometryObject::InverseTransformNormal(glm::vec3 normal){
	//Get the inverse of the transform
	glm::mat4 inverseMat = glm::inverse(transform);
	//Get the transpose of the inverse
	glm::mat4 inverseTransposeMat = glm::transpose(inverseMat);

	//If the normalwasn't normalized we do it again here
	normal = glm::normalize(normal);

	glm::vec4  transformNormal = glm::vec4(normal, 1);

	//Transform the normal
	transformNormal = inverseTransposeMat * transformNormal;

	//Dehomogenize normal if necessary
	transformNormal /= transformNormal.w;

	//normalize the normal again
	transformNormal = glm::normalize(transformNormal);

	//Return the now transformed normal
	return glm::vec3(transformNormal);


}


Quad::Quad(){
	for (int i = 0; i < 4; i++){
		points[i] = glm::vec3(0.0, 0.0, 0.0);
	}

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}
Quad::Quad(glm::vec3 inputPoints[4]){
	//assing each point
	for (int i = 0; i < 4; i++){
		points[i] = inputPoints[i];
	}

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}


//Define intersections of quads
HitInfo Quad::Intersect(Ray ray){

	//Create the hit info to return
	HitInfo hitInfo = HitInfo();
	hitInfo.t = 200;

	ray = TransformRay(ray);


	//We must assume the quad is coplanar
	glm::vec3 quadPlaneNorm = glm::cross((points[1] - points[0]), (points[2] - points[0]));
	quadPlaneNorm = glm::normalize(quadPlaneNorm);
	//A point M belongs to this plane iff it satifies the eqation n dot (M-points[0]) = 0
	//A point M belongs to the ray iff it can be written: M = R1 + t * dR with dR = R2 - R1
	//Thats just the equation of a ray
	//Compute the ray/plane intersection(equate the two previous equations):
	//t is the t in the ray equation
	float t = glm::dot(-quadPlaneNorm, (ray.origin - points[0])) / (glm::dot(quadPlaneNorm, ray.direction));

	//Intersection point M
	glm::vec3 M = (ray.origin + t*ray.direction);

	/*
	* Project the vector M - S1 onto the two vectors S2 - S1 and S3 - S1 (the square edges 
	* starting from S1), this gives two numbers (u, v):
	*/

	//No the actual u and v check but the names work with the rest of the function
	bool uCheck = -1 <= M[0] && 1 >= M[0];
	bool vCheck = -1 <= M[1] && 1 >= M[1];

	//Simplify, I know the dimensions of the quad I am inputting and that it is not tilted
	//If -1 <= M[0] <= 1 and -1 <= M[1] <= 1 then the point is in the quad

	//float u = glm::dot((M - points[0]), (points[1] - points[0]));
	//float v = glm::dot((M - points[0]), (points[2] - points[0]));
	/*
	*	If 0 <= u <= | S2 - S1 | ^ 2 and 0 <= v <= | S3 - S1 | ^ 2, then the point of intersection
	*	M lies inside the square, else it's outside.
	*/
	//GLM length is supposed to get magnitude
	/*
	glm::vec3 uVec = points[1] - points[0];
	float uMag = sqrt((uVec.x*uVec.x) + (uVec.y*uVec.y) + (uVec.z*uVec.z));
	bool uCheck = (0 <= u) && (u <= pow(uMag,2.0));

	glm::vec3 vVec = points[2] - points[0];
	float vMag = sqrt((vVec.x*vVec.x) + (vVec.y*vVec.y) + (vVec.z*vVec.z));
	bool vCheck = (0 <= v) && (v <= pow(vMag, 2.0));
	*/
	//For now just color it all red
	if (uCheck && vCheck){


		//Assign the values of hit info
		hitInfo.t = t;
		hitInfo.raycast = ray;
		hitInfo.position = ray.origin + ray.direction*t;
		glm::vec3 normal = quadPlaneNorm;
		normal = InverseTransformNormal(normal);
		hitInfo.normal = normal;
		hitInfo.collisionObject = this;
	}

	

	return hitInfo;

}

/*Triangle*/
Triangle::Triangle(){
	for (int i = 0; i < 4; i++){
		points[i] = glm::vec3(0.0, 0.0, 0.0);
	}

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}

Triangle::Triangle(glm::vec3 inputPoints[3]){
	//assing each point
	for (int i = 0; i < 3; i++){
		points[i] = inputPoints[i];
	}

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}

HitInfo Triangle::Intersect(Ray ray){

	//Create the hit info to return
	HitInfo hitInfo = HitInfo();
	hitInfo.t = 200;

	hitInfo.raycast = ray;
	ray = TransformRay(ray);

	//Get the normal of the triangle
	glm::vec3 triNormal = glm::cross((points[2] - points[0]), (points[1] - points[0]));
	triNormal = glm::normalize(triNormal);

	//Gor reference with the equation as it was written in the lecture
	glm::vec3 vP0 = ray.origin;
	glm::vec3 vP1 = ray.direction;

	/*
	* plane = vP*vNorm - vA*vNorm = 0
	* combine with ray equation
	* ray = vP = vP0 + (vP1)t
	* (vP0 + (vP1)t)*vNorm = vA*vNorm
	* solve for t that becomes
	* t = ((vA*vNorm)-(vP0*vNorm))/(vP1*vNorm)
	*/

	float t = (glm::dot(points[0], triNormal) - glm::dot(vP0, triNormal)) / glm::dot(vP1, triNormal);

	//with t now we can define the point P we will test for being inside the triangle
	glm::vec3 P = ray.origin + t*ray.direction;

	/*
	* Now ray-triangle
	* Use barycentric coordinates(alpha, beta, gamma)
	*/
	//using http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates

	float alpha, beta, gamma;
	
	//I found this on the internet
	float areaABC, areaPBC, areaPCA;
	areaABC = glm::dot(triNormal, glm::cross(points[1] - points[0], points[2] - points[0]));
	areaPBC = glm::dot(triNormal, glm::cross(points[1] - P, points[2] - P));
	areaPCA = glm::dot(triNormal, glm::cross(points[2] - P, points[0] - P));

	alpha = areaPBC / areaABC;
	beta = areaPCA / areaABC;
	gamma = 1.0f - alpha - beta;

	//alpha beta and gamma need to add to 1 within some threshold to make up for floating
	//point precision
	float threshold = 0.01;

	//Do alpha beta and gamma add up to 1
	bool withinThreshold = (alpha + beta + gamma >= 1.0 - threshold) && (alpha + beta + gamma <= 1.0 + threshold);

	//Are alpha beta and gamma less than 1
	bool lessThanEqualTo = alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0;


	//color it red
	if (withinThreshold && lessThanEqualTo){

		//Assign the values of hit info
		hitInfo.t = t;
		hitInfo.position = P;
		glm::vec3 normal = triNormal;
		normal = InverseTransformNormal(normal);
		hitInfo.normal = normal;
		hitInfo.collisionObject = this;
	}

	return hitInfo;
}


//Basic constructor for a sphere giving default values
Sphere::Sphere(){
	position = glm::vec3(0, 0, 0);
	radius = 1;

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}

//Constructor that actually sets the parameters of the sphere
Sphere::Sphere(glm::vec3 pos, float rad){
	position = pos;
	radius = rad;

	//Set the defaults for material properties
	diffuse = glm::vec3(0, 0, 0);
	shininess = 0.0;
	specular = glm::vec3(0, 0, 0);
	emission = glm::vec3(0.0, 0.0, 0.0);
}


//Define how intersection will work for a sphere
HitInfo Sphere::Intersect(Ray ray){

	//Create the hit info to return
	HitInfo hitInfo = HitInfo();
	hitInfo.t = 200;
	hitInfo.raycast = ray;

	ray = TransformRay(ray);

	//Ray = P = origin + direction*time
	//Sphere = dot((P - Center), (P - Center) - r^2 = 0

	//Then substitute
	//dot((RayOrigin + RayDir*t - SphereCenter), (RayOrigin + RayDir*t - SphereCenter) - r^2 = 0
	//Simplify
	//t^2(dot(RayDir, RayDir)) + 2t(dot(RayDir, RayOrigin - SphereCenter) + dot(RayOrigin - SphereCenter, RayOrigin - SphereCenter) - SphereRadius^2 = 0
	float t = 0;

	float a = glm::dot(ray.direction, ray.direction);

	float b = 2 * glm::dot(ray.direction, ray.origin - position);

	float c = glm::dot(ray.origin - position, ray.origin - position) - (radius*radius);

	//Check discriminant first, if negative no intersection occurs
	float discriminant = (b*b - 4 * a*c);

	//No intersection so return early
	if (discriminant < 0){
		return hitInfo;
	}

	//Roots are not complex so lets find them
	float root1 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
	float root2 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
	
	if (root1 == root2){
		return hitInfo;
	}

	glm::vec3 intersectionPoint;

	//Now depending on signes of roots, 
	//2 positive roots, pick smaller one
	if (root1 > 0 && root2 > 0){
		if (root1 > root2){
			//Pick root2
			intersectionPoint = ray.origin + ray.direction*root2;
			hitInfo.t = root2;
			hitInfo.position = intersectionPoint;
			hitInfo.collisionObject = this;
			//Normal for sphere is just the normalize vector of the intersection point - the sphere center
			glm::vec3 normal = glm::normalize(intersectionPoint - position);
			normal = InverseTransformNormal(normal);
			hitInfo.normal = normal;
			return hitInfo;
		}
		else if(root2 > root1){
			//Pick root1
			intersectionPoint = ray.origin + ray.direction*root1;
			hitInfo.t = root1;
			hitInfo.position = intersectionPoint;
			hitInfo.collisionObject = this;
			//Normal for sphere is just the normalize vector of the intersection point - the sphere center
			glm::vec3 normal = glm::normalize(intersectionPoint - position);
			normal = InverseTransformNormal(normal);
			hitInfo.normal = normal;
			return hitInfo;

		}
	}

	//One positive root, one negative, ray origin is inside the sphere
	if (root1 > 0 && root2 < 0){
		//Pick root1
		intersectionPoint = ray.origin + ray.direction*root1;

		hitInfo.t = root1;
		hitInfo.position = intersectionPoint;
		hitInfo.collisionObject = this;
		//Normal for sphere is just the normalize vector of the intersection point - the sphere center
		glm::vec3 normal = glm::normalize(intersectionPoint - position);
		normal = InverseTransformNormal(normal);
		hitInfo.normal = normal;
		return hitInfo;

	}
	else if (root2 > 0 && root1 < 0){
		//Pick root2
		intersectionPoint = ray.origin + ray.direction*root2;

		hitInfo.t = root2;
		hitInfo.position = intersectionPoint;
		hitInfo.collisionObject = this;
		//Normal for sphere is just the normalize vector of the intersection point - the sphere center
		glm::vec3 normal = glm::normalize(intersectionPoint - position);
		normal = InverseTransformNormal(normal);
		hitInfo.normal = normal;
		return hitInfo;
	}


	//Assign the values of hit info
	hitInfo.t = t;
	hitInfo.position = intersectionPoint;
	//Normal for sphere is just the normalize vector of the intersection point - the sphere center
	glm::vec3 normal = glm::normalize(intersectionPoint - position);
	normal = InverseTransformNormal(normal);
	hitInfo.normal = normal;
	hitInfo.collisionObject = this;

	return hitInfo;

}
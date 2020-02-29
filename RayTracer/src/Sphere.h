#pragma once
#include "Object.h"

class Sphere : public Object
{
public:
	glm::vec3 pos;
	float radius;
	bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

	
	Sphere(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float radius, float shininess);
	float Intersect(const Ray &ray);
	glm::vec3 getNormal(const glm::vec3 &point);
};

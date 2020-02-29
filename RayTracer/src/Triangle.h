#pragma once
#include "Object.h"
#define EPSILON 0.000001

class Triangle : public Object
{
public:
	glm::vec3 v1, v2, v3;
	glm::vec3 normal;

	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	float Intersect(const Ray &ray);
	glm::vec3 getNormal(const glm::vec3 &point);
};
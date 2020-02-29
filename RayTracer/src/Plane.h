#pragma once
#include "Object.h"

class Plane : public Object
{
public:
	glm::vec3 position, normal;

	Plane(glm::vec3 normal, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	float Intersect(const Ray & ray);
	glm::vec3 getNormal(const glm::vec3& point);
};

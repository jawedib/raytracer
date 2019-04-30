#include "Plane.h"


Plane::Plane(glm::vec3 normal, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	Object(ambient, diffuse, specular, shininess), normal(normal), position(position)
{
}

float Plane::Intersect(const Ray &ray) 
{
	auto a = glm::dot(ray.direction, normal);

	if (a==0)
	{
		return -1;
	}
	
	return glm::dot(position - ray.origin, normal) / a;
	
}

glm::vec3 Plane::getNormal(const glm::vec3& point) 
{
	return normal;
}
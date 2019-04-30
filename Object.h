#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Ray.h"

//Parent for all the shapes. Two virtual functions where functionality depends on type of object
class Object
{
public:
	glm::vec3 ambient, diffuse, specular;
	float shininess;

	virtual float Intersect(const Ray&);
	virtual glm::vec3 getNormal(const glm::vec3 & point);
	Object(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};

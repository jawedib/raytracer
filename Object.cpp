#include "Object.h";

float Object::Intersect(const Ray &)
{
	return 0.0f;
}

glm::vec3 Object::getNormal(const glm::vec3 & point)
{
	return glm::vec3();
}

Object::Object(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{

}

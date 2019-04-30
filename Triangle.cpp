#include "Triangle.h"

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	Object(ambient, diffuse, specular, shininess), v1(v1), v2(v2), v3(v3)
{
	glm::vec3 e1 = v2 - v1;
	glm::vec3 e2 = v3 - v1;
	normal = cross(e1, e2);
}


float Triangle::Intersect(const Ray &ray)
{
	glm::vec3 e1, e2;
	glm::vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	e1 = v2 - v1;
	e2 = v3 - v1;
	P = glm::cross(ray.direction, e2);
	
	det = glm::dot(e1, P);

	if (det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.f / det;

	T = ray.origin - v1;

	u = glm::dot(T, P) * inv_det;
	if (u<0.f || u > 1.f) return 0;

	Q = glm::cross(T, e1);

	v = glm::dot(ray.direction, Q)*inv_det;
	if (v<0.f || u + v > 1.f) return 0;

	t = glm::dot(e2, Q) * inv_det;
	if (t>EPSILON)
	{
		return t;
	}

	return 0;

}

glm::vec3 Triangle::getNormal(const glm::vec3 &point) 
{
	return normal;
}

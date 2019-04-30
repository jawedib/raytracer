#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float radius, float shininess) :
	Object(ambient, diffuse, specular, shininess), pos(position), radius(radius)
{
}


float Sphere::Intersect(const Ray& ray)
{
	float t0, t1;  // solutions for t if the ray intersects 
	glm::vec3 dir = ray.direction;
	glm::vec3 L = ray.origin-pos;
	auto a = glm::dot(dir, dir);
	auto b = 2 * glm::dot(dir, L);
	auto c = glm::dot(L,L) - pow(radius,2);
	if (!solveQuadratic(a, b, c, t0, t1)) return 0;
	if (!solveQuadratic(a, b, c, t0, t1)) return 0;

	if (t0 > t1) std::swap(t0, t1);
	if (t0<0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead (INSTEAD WE DON'T BECAUSE OF THE PROJECT REQUIREMENT "Back-facing surfaces should not be considered")
		if (t0 < 0) return 0; // both t0 and t1 are negative
	}
	return t0;
}

glm::vec3 Sphere::getNormal(const glm::vec3 &point)
{
	auto normal = normalize(point - pos);
	return normal;
}


bool Sphere::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}
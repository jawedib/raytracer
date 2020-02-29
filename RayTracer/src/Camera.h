#pragma once
#include "glm/glm.hpp"
#include <iostream>

class Camera
{
public:
	glm::vec3 pos;
	float fov, focal_length, aspect_ratio;
	
	Camera();
	Camera(glm::vec3, float, float, float);
};
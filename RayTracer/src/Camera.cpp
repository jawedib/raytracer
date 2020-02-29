#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 pos, float fov, float focal_length, float aspect_ratio) :
	pos(pos), fov(fov), focal_length(focal_length), aspect_ratio(aspect_ratio)
{
	//std::cout << "fov: " + std::to_string(fov) + "\n";
}
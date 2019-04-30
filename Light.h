#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include <string>

class Light
{
public:
	glm::vec3 position, color;

	Light(glm::vec3 position, glm::vec3 color);
};
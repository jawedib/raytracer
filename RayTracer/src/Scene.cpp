#include "Scene.h"
#include <string>
#include <iostream>
#include "glm/detail/type_mat.hpp"


bool Scene::newScene()
{
	// Calculate height
	height = tan(glm::radians(camera->fov / 2)) * camera->focal_length;
	//Calculate width
	width = camera->aspect_ratio*height;

	return true;
}

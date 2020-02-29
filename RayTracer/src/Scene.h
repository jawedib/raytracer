#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Camera.h"
#include "Plane.h"
#include "Light.h"

class Scene
{
public:
	int width;
	int height;
	Camera* camera;
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	
	bool newScene();

	
};
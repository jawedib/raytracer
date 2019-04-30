#include <iostream>
#include <vector>
#include <sstream>
#include "Scene.h"
#include <iterator>
#include <fstream>

#include "Scene.h"
#pragma once
class InputReader
{
public:
	InputReader(std::string file);
	Scene scene;

	std::ifstream input;
	void newCamera();
	void newPlane();
	void newTriangle();
	void newMesh();
	void newSphere();
	void newLight();
	static glm::vec3 parseVector(std::string line);
	static float parseFloat(std::string line);
};
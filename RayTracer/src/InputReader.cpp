#include "InputReader.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "OBJloader.h"
#include <iostream>

void InputReader::newSphere()
{
	glm::vec3 position, ambient, diffuse, specular;
	float radius, shininess;

	std::string line;

	getline(input, line);
	position = parseVector(line);

	getline(input, line);
	radius = parseFloat(line);

	getline(input, line);
	ambient = parseVector(line);

	getline(input, line);
	diffuse = parseVector(line);

	getline(input, line);
	specular = parseVector(line);

	getline(input, line);
	shininess = parseFloat(line);

	std::cout << "Sphere of radius " << radius << std::endl;
	//Create and push the plane object in the scene object vector 
	scene.objects.push_back(new Sphere(position, ambient, diffuse, specular, radius, shininess));
}


void InputReader::newPlane()
{
	glm::vec3 normal, position, ambient, diffuse, specular;
	float shininess;

	std::string line;

	getline(input, line);
	normal = parseVector(line);

	getline(input, line);
	position = parseVector(line);

	getline(input, line);
	ambient = parseVector(line);

	getline(input, line);
	diffuse = parseVector(line);

	getline(input, line);
	specular = parseVector(line);

	getline(input, line);
	shininess = parseFloat(line);

	std::cout << "Plane" << std::endl;

	//Create and push the plane object in the scene object vector 
	scene.objects.push_back(new Plane(normal, position, ambient, diffuse, specular, shininess));
}

void InputReader::newTriangle()
{
	glm::vec3 v1, v2, v3, ambient, diffuse, specular;
	float shininess;

	std::string line;

	getline(input, line);
	v1 = parseVector(line);

	getline(input, line);
	v2 = parseVector(line);

	getline(input, line);
	v3 = parseVector(line);

	getline(input, line);
	ambient = parseVector(line);

	getline(input, line);
	diffuse = parseVector(line);

	getline(input, line);
	specular = parseVector(line);

	getline(input, line);
	shininess = parseFloat(line);

	scene.objects.push_back(new Triangle(v1, v2, v3, ambient, diffuse, specular, shininess));
}

void InputReader::newMesh() 
{
	std::string objectFile;
	glm::vec3 ambient, diffuse, specular, translate;
	float shininess;

	std::string line;

	//parsing the object name
	getline(input, line);
	objectFile = (line);
	objectFile = objectFile.substr(6,50);
	const char *cstr = objectFile.c_str(); //converted to char

	getline(input, line);
	//std::cout << line << std::endl;
	ambient = parseVector(line);

	getline(input, line);
	//std::cout << line << std::endl;
	diffuse = parseVector(line);

	getline(input, line);
	//std::cout << line << std::endl;
	specular = parseVector(line);

	getline(input, line);
	//std::cout << line << std::endl;
	translate = parseVector(line);

	getline(input, line);
	//std::cout << line << std::endl;
	shininess = parseFloat(line);

	std::cout << objectFile << std::endl;

	//Same OBJloader we have used for previous assignments
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ(cstr, vertices, normals, UVs); //read the vertices from the cube.obj file

	//std::cout << vertices.size() << std::endl;
	int size = vertices.size();
	//this loop creates a new triangle from every 3 vertices in from the vertices vector aka the mesh
	for (int i = 0; i < size; i++) {
		scene.objects.push_back(new Triangle(vertices[i]+ translate, vertices[i + 1] + translate, vertices[i + 2] + translate, ambient, diffuse, specular, shininess));
		i++;
		i++;
	}
}


void InputReader::newLight()
{
	glm::vec3 position, color;

	std::string line;

	getline(input, line);
	std::cout << "Light at " << line << std::endl;
	position = parseVector(line);

	getline(input, line);
	color = parseVector(line);

	scene.lights.push_back(new Light(position, color));
}


glm::vec3 InputReader::parseVector(std::string line)
{
	std::stringstream ss(line);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	float x, y, z;
	x = stof(vstrings[1]);
	y = stof(vstrings[2]);
	z = stof(vstrings[3]);

	return glm::vec3(x, y, z);

}

void InputReader::newCamera()
{
	glm::vec3 position;
	float fov, aspect_ratio, focal_length;
	std::string line;

	getline(input, line);
	position = parseVector(line);

	getline(input, line);
	fov = parseFloat(line);

	getline(input, line);
	focal_length = parseFloat(line);

	getline(input, line);
	aspect_ratio = parseFloat(line);

	std::cout << "Camera" << std::endl;

	//scene.camera = new Camera(position, fov, focal_length, aspect_ratio);

	//Create and push the camera object into the scene
	scene.camera = new Camera(position + glm::vec3(0,0,7), fov, focal_length, aspect_ratio);
}

float InputReader::parseFloat(std::string line)
{
	std::stringstream ss(line);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	return stof(vstrings[1]);
}


InputReader::InputReader(std::string file)
{
	input.open(file);
	std::cout << "Reading " << file << std::endl;
	if (input.is_open())
	{
		std::string line;
		getline(input, line);
		std::cout << line << " objects total: "<< std::endl;
		int objectCount = stoi(line); // Read number of objects in scene

		//I had to create this while loop because for whatever reason it wasn't parsing through all the objects in the scene file and skipping sometimes. The while loop makes sure it reaches all of them.
		while(objectCount !=0)
		{
			getline(input, line); // Read object type
			{
				if (line == "camera") {
					newCamera();
					objectCount--;
				}
				else if (line == "plane") {
					newPlane();
					objectCount--;
				}
				else if (line == "triangle") {
					newTriangle();
					objectCount--;
				}
				else if (line == "mesh") {
					newMesh();
					objectCount--;
				}
				else if (line == "sphere") {
					newSphere();
					objectCount--;
				}
				else if (line == "light") {
					newLight();
					objectCount--;
				}
			}
		}
		if (scene.camera != nullptr)
		{
			scene.newScene();//building new scene after reading all objects in scene
		}
	}
}

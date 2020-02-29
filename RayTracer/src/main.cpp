#include "Scene.h"
#include "InputReader.h"
#include "cimg/CImg.h"
#include <thread>
#include <iostream>
#include <vector>


const double accuracy = 0.0005;
const glm::vec3 cam_dir = glm::vec3(0, 0, -1);
const glm::vec3 cam_right = glm::vec3(1, 0, 0);
const glm::vec3 cam_down = glm::cross(cam_dir, cam_right);

//Colors going beyond 1 or 0 need to be checked
float normalizeColor(const float &color)
{
	if (color > 1.f) return 1.f;

	if (color < 0) return 0.f;
	
	return color;
}

void writeImage(glm::vec3* pixels, const int &width, const int &height, const std::string &FileName)
{
	cimg_library::CImg<float> image(width, height, 1, 3, 0);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int index = x * height + y;
			float red = pixels[index].x * 255;
			float green = pixels[index].y * 255;
			float blue = pixels[index].z * 255;
			image(x, height-y-1, 0) = red;
			image(x, height-y-1, 1) = green;
			image(x, height-y-1, 2) = blue;
		}
	}
	//image = image.rotate(180,0,0);
	//image.save(("output/" + FileName + ".bmp").c_str());
	//cout <<"\n"<< FileName << ".bmp created!" << endl;

	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}

glm::vec3 getColor(const glm::vec3& intersection_position, const glm::vec3& intersection_direction, const std::vector<Object*>& objects, const std::vector<Light*>& lights, const int& index)
{
	
	//object ambient color which gets applied regardless of light hitting the object
	glm::vec3 result = glm::vec3(0, 0, 0);
	//glm::vec3 result = objects.at(index)->ambient;
	
	//retrieving other light properties
	glm::vec3 objdiffuse = objects.at(index)->diffuse;
	glm::vec3 objspecular = objects.at(index)->specular;
	float objshininess = objects.at(index)->shininess;

	glm::vec3 normal = normalize(objects.at(index)->getNormal(intersection_position));

	float minDistance;

	// Checking each light if it contributes to this pixel
	for (int i = 0; i < lights.size(); i++)
	{
		
		glm::vec3 light_pos = lights[i]->position;
		glm::vec3 light_dir = normalize(light_pos - intersection_position);
		float cosine = dot(normal, light_dir);

		if (cosine > 0)
		{
			bool shadowed = false;
			float distance_to_light = length(light_pos - intersection_position);

			//creating shadow ray origin
			glm::vec3 shadow_ray_origin = intersection_position + light_dir * (float)0.1;

			//firing the shadow ray towards the light
			Ray shadow_ray(shadow_ray_origin, light_dir);

			minDistance = 10000.00;
			int closestObject = -1;

			//this for loop goes through the scene objects one by one and checks if any of them obstruct the shadow ray from reaching the light
			for (int i = 0; i < objects.size(); i++)
			{
				float distance = objects.at(i)->Intersect(shadow_ray);
				//if the intersection distance is less than minimum allowed distance(10,000 in this case; an arbitrary unrealistic high number)
				if (distance<minDistance && distance > accuracy)
				{
					minDistance = distance; // we record the length of the shadow ray here to the obstructing object
					closestObject = i; //in the end, if there was an obstruction, the ith object in the vector is the closest to the shadow ray
				}
			}

			//if an object exists that obstructed the shadow ray...
			if (closestObject >= 0)
			{
				// if the obstruction distance is not greater than distance to light aka the obstruction is happening BEHIND the light 
				if (minDistance <= distance_to_light)
				{
					//that means this pixel is not receiving any contribution from this particular light.
					shadowed = true;
					break;
				}
			}

			//if NOT shadowed
			if (!shadowed) {
				glm::vec3 final_color = glm::vec3(0, 0, 0);

				glm::vec3 diffuse = objdiffuse * std::max((float)0, cosine);

				glm::vec3 lightAngle = glm::normalize(2 * dot(light_dir, normal) * normal - light_dir);
				float dotProduct = dot(lightAngle, -intersection_direction);

				// if dotProcut is positive, add light the lights specular AND diffuse properties
				if (dotProduct > 0) {
					glm::vec3 specular = objspecular * std::max((float)0, pow(dotProduct, objshininess));
					final_color.x += lights[i]->color.x*(diffuse.x + specular.x);
					final_color.y += lights[i]->color.y*(diffuse.y + specular.y);
					final_color.z += lights[i]->color.z*(diffuse.z + specular.z);
				}
				else // else, only diffuse
				{
					final_color.x += lights[i]->color.x*diffuse.x;
					final_color.y += lights[i]->color.y*diffuse.y;
					final_color.z += lights[i]->color.z*diffuse.z;
				}
				//a pixel may receive light from more than once source, so keep adding
				result += final_color;
			}
		}
	}
	
	//REFLECTION CALCULATION
	glm::vec3 intersection_position2;
	glm::vec3 reflection_ray_dir;

	float minReflectionDistance = 10000000000;
	int closestReflectionObject = -1;

	for (int i = 0; i < objects.size(); i++) {
		reflection_ray_dir = glm::normalize(2 * dot(-intersection_direction, normal) * normal + intersection_direction);
		Ray reflection_ray(intersection_position, reflection_ray_dir);

		float distance = objects.at(i)->Intersect(reflection_ray);
		if (distance<minReflectionDistance && distance > accuracy)
		{
			minReflectionDistance = distance; // we record the length of the reflection ray here to the intersecting object
			closestReflectionObject = i; //in the end, if there was an intersection, the ith object in the vector is the closest to the shadow ray
			intersection_position2 = intersection_position + (reflection_ray_dir * minReflectionDistance);
		}
	}

	if (closestReflectionObject >= 0)
	{
		//the variable 'target' is where the reflection ray bounced to, so in the end we mix the color depending on the shininess of the object, the
		//object color and the reflection color
		float reflect = glm::clamp(((objshininess / 20)), float(0), float(1.00)); //OPTIMAL SETTINGS
		glm::vec3 target = getColor(intersection_position2, reflection_ray_dir, objects, lights, closestReflectionObject);
		result = ((result * (1 - reflect)) + (target * reflect));
	}
	
	return glm::vec3(normalizeColor(result.x), normalizeColor(result.y), normalizeColor(result.z));
}



// Compute the colour of pixels in the specified range
void rayTrace(int windowwidth, int windowheight, const Scene &scene, glm::vec3* pixels)
{
	float width = scene.width;
	float height = scene.height;
	float aspect_ratio = scene.camera->aspect_ratio;
	glm::vec3 cam_pos = scene.camera->pos;
	float fov = glm::radians(scene.camera->fov);

	for (int x = 0; x < windowwidth; x++)
	{
		for (int y = 0; y < windowheight; y++)
		{
			float xamt, yamt;
			glm::vec3 intersection_color = glm::vec3(0, 0, 0);

			xamt = ((x + 0.5) / width)*aspect_ratio - (((width - height) / (float)height) / 2);
			yamt = ((height - y) + 0.5) / height;

			//creating and firing camera ray through then viewing plane
			glm::vec3 camray_dir = glm::normalize(cam_dir + cam_right * static_cast<float>(xamt - 0.5) + cam_down * static_cast<float>(yamt - 0.5));
			Ray cam_ray(cam_pos, camray_dir);

			int closestObject = -1;
			float minDistance = 10000;

			// go through each object to see if the ray is being obstructed by any object
			for (int i = 0; i < scene.objects.size(); i++)
			{
				float distance = scene.objects[i]->Intersect(cam_ray);
				//if the intersection distance is less than minimum allowed distance(10,000 in this case; an arbitrary unrealistic high number)
				if (distance< minDistance && distance > accuracy)
				{
					minDistance = distance; //record the minimum distance 
					closestObject = i; // record the object index that is closest
				}
			}

			//if there's an obstruction
			if (closestObject >= 0)
			{
				//get the intesection position
				glm::vec3 intersection_position = cam_pos + (camray_dir * minDistance);

				//get the intersection pixel color from getColor function
				intersection_color = getColor(intersection_position, camray_dir, scene.objects, scene.lights, closestObject);

				//intersection_color = glm::vec3(0.5, 0.5, 0.5); //for debugging
			}
			else
			{
				//ray hits nothing so we do nothing/use default intersection color; black in this case.
			}

			glm::vec3 result(0, 0, 0);
			result.x = intersection_color.x;
			result.y = intersection_color.y;
			result.z = intersection_color.z;

			int index = x * height + y;
			pixels[index] = result;
		}
	}
}

int main()
{	
	std::string fileName = "scene1.txt";
	//std::cout << "Enter the scene file name without the '.txt' : ";
	//std::cin >> FileName;
	InputReader* inputreader = new InputReader(fileName);

	Scene scene = inputreader->scene;
	delete inputreader;

	int width = scene.width;
	int height = scene.height;
	int allPixels = width * height;
	glm::vec3* pixelvec = new glm::vec3[allPixels];
	
	const clock_t begin_time = clock();


	rayTrace(width, height, scene, pixelvec);

	
	std::cout << "Reached end of raycast loop after " << float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds" << std::endl;
	writeImage(pixelvec, width, height, fileName);

	delete pixelvec;
	

	return 0;
}


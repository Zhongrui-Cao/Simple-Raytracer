#ifndef MYCLASS_H
#define MYCLASS_H

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

class Ray {
public:
	glm::vec3 origin, direction;
	float tmin, tmax;

	Ray(glm::vec3 ori, glm::vec3 dir) : origin(ori), direction(dir)
	{};
};

class Camera {
public:
	glm::vec3 position;
	float fovx, fovy;
	int h, w;

	Camera() = default;
	Camera(glm::vec3 position, float fovy, int w, int h)
		: position(position), fovy(fovy), h(h), w(w)
	{
		fovx = 2.f * atan( ((float)w / (float)(h)) * tan(fovy / 2.f) );
	};
};

//infusing triangle, circle, vertex, material classes
struct Object {
	std::string type;
	glm::vec3 v0, v1, v2;
	float radius;
	float size;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emission;
	int shininess;
	glm::mat4 transform;
};

class Intersection {
public:

	float hitDistance;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 direction;
	//Material* Mtl;
	Object object;
	int recursionDepth = 0;

	//Color Shade;
	Intersection() = default;
	Intersection(glm::vec3 position, glm::vec3 normal, float hitdistance, Object object, int depth)
		: hitDistance(hitdistance), position(position), normal(normal), object(object), recursionDepth(depth) {};
};

class Scene {
public:
	std::vector<Object> objects;
};

enum LightType {POINT, DIRECTIONAL};
class Light {
public:
	glm::vec3 position, color;
	LightType lighttype;

	Light() = default;
	Light(LightType lighttype, float x, float y, float z, float r, float g, float b)
		: position(x, y, z), color(r, g, b), lighttype(lighttype) {};
};

class Image {
public:
	std::vector<std::vector<glm::vec3>> pixmat;
	unsigned char* pixels;
	int h, w;

	Image(int w, int h) : h(h), w(w)
	{
		for (int i = 0; i < h; i++)
		{
			std::vector<glm::vec3> row(w);
			pixmat.push_back(row);
		}
		pixels = new unsigned char[w*h*3];
	}
};

#endif

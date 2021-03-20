#ifndef RAYTRACER_H
#define RAYTRACER_H
#endif

#include "variables.h"
#include <iostream>
#include <string>
#include <cmath>

Ray RayThruPixel(const Camera& cam, int i, int j);
float GetIntersectDist(const Ray& ray, const Object& obj, glm::vec3* normal);
bool Intersect(const Ray& ray, const Scene& scene, const Intersection* hit);
Image* Raytrace(const Camera& cam, const Scene& scene, int w, int h);
glm::vec3 FindColor(const Intersection& hit, const Scene& scene, glm::vec3 ray_start);
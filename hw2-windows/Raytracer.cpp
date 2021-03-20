#include "raytracer.h"
#include <glm/ext.hpp>
#include <iostream>
#include "progressbar.h"
using namespace std;


const float infinity = std::numeric_limits<float>::infinity();
const float EpsilonSphere = .0100;
const float EpsilonTri = .00010;

Ray RayThruPixel(const Camera& cam, int i, int j) {
	glm::vec3 w = glm::normalize(eye - center);
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = glm::cross(w, u);

	float alpha = tan(cam.fovx / 2.f) * ( ( ((float)j + 0.5) - ((float)cam.w / 2.f) ) / ((float)cam.w / 2.f) );
	float beta  = tan(cam.fovy / 2.f) * ( ( ((float)i + 0.5) - ((float)cam.h / 2.f) ) / ((float)cam.h / 2.f) );

	glm::vec3 d = glm::normalize(alpha * u + beta * v - w);

	return Ray(cam.position, d);
}

float GetIntersectDist(const Ray& ray, const Object& obj, glm::vec3* normal){
	
	if (obj.type == "tri") {

		*normal = glm::normalize( glm::cross((obj.v1 - obj.v0),(obj.v2 - obj.v0)) );
		float normray = glm::dot(ray.direction, *normal);
	
		//check if ray and plane parallel
		if (fabs(normray) < EpsilonTri)
			return infinity;

		float d = glm::dot(*normal, obj.v0);
		float t = (d - glm::dot(*normal, (ray.origin + EpsilonTri * ray.direction) )) / normray;

		//ray is behind
		if (t < 0.f)
			return infinity;

		glm::vec3 p = ray.origin + t * ray.direction;  //intersection point

		float lamda0; // vector perpendicular to plane for testing in/out
		glm::vec3 edge0 = obj.v1 - obj.v0;
		glm::vec3 vp0 = p - obj.v0;
		lamda0 = glm::dot(*normal, glm::cross(edge0, vp0));

		glm::vec3 edge1 = obj.v2 - obj.v1;
		glm::vec3 vp1 = p - obj.v1;
		float lamda1 = glm::dot(*normal, glm::cross(edge1, vp1));

		glm::vec3 edge2 = obj.v0 - obj.v2;
		glm::vec3 vp2 = p - obj.v2;
		float lamda2 = glm::dot(*normal, glm::cross(edge2, vp2));

		if (lamda0 >= 0 && lamda1 >= 0 && lamda2 >= 0)
			return t;
		
		return infinity;

	}
	else if (obj.type == "sphere") {

		float hitDist;
		glm::vec3 cen = obj.v0;
		glm::mat4 invTrans = glm::inverseTranspose(obj.transform);
		glm::vec3 p0 = glm::vec3(glm::inverse(obj.transform) * (glm::vec4(ray.origin, 1) + EpsilonSphere * glm::vec4(ray.direction, 0)));
		//glm::vec3 p0 = glm::vec3(glm::inverse(obj.transform) * (glm::vec4(ray.origin, 1)));
		glm::vec3 p1 = glm::vec3(glm::normalize(glm::inverse(obj.transform) * glm::vec4(ray.direction, 0)));
		Ray tranRay(p0, p1);

		glm::vec3 oc = p0 - cen;
		float a = glm::dot(p1, p1);
		//simplify
		float halfb = glm::dot(p1, oc);
		float b = 2.0f * halfb;
		float c = glm::dot(oc, oc) - obj.radius * obj.radius;

		float discriminant = b * b - 4 * a * c;

		//two roots
		float rootminus = (-b - sqrt(discriminant)) / (2.f * a);
		float rootplus  = (-b + sqrt(discriminant)) / (2.f * a);
		
		//complex root
		if (discriminant < 0) {
			return infinity;
		}
		else if (discriminant > 0.f){
			//choose positive one if 2 real roots
			if (((rootminus > 0) && (rootplus < 0)) || ((rootminus < 0) && (rootplus > 0))) {
				hitDist = (rootminus > 0 ? rootminus : rootplus);
			}
			//choose the smaller one if 2 positive roots
			else if (rootminus > 0 && rootplus > 0){
				hitDist = (rootminus > rootplus ? rootplus : rootminus);
			}
			else {
				hitDist = infinity;
			}
		}
		else {
			hitDist = rootminus;
		}
		
		glm::vec3 p = (p0 + p1 * hitDist);
		glm::vec3 interce = glm::vec3(obj.transform * glm::vec4(p, 1));
		float newhitDist = glm::length(interce - ray.origin);
		*normal = glm::normalize(glm::vec3(invTrans * glm::vec4(glm::normalize(p - cen), 0)));
		return newhitDist;
	}
	return infinity;
}

bool Intersect(const Ray& ray, const Scene& scene, Intersection* hit) {
	hit->hitDistance = infinity;
	glm::vec3 normal;
	float temphit;
	// go through all objects in scene and find closest intersection
	for (int i = 0; i < numobjects; i++) {
		temphit = GetIntersectDist(ray, scene.objects[i], &normal);
		if (temphit < hit->hitDistance) {
			hit->hitDistance = temphit;
			hit->object = scene.objects[i];
			hit->normal = normal;
		}
	}
	// check if hit
	if (hit->hitDistance < infinity) {
		hit->position = ray.origin + hit->hitDistance * ray.direction;
		return true;
	}
	return false;
}

glm::vec3 FindColor(const Intersection& hit, const Scene& scene, glm::vec3 ray_start) {

	glm::vec3 lighting(0.f);
	if (hit.hitDistance < infinity) {
		lighting = hit.object.ambient + hit.object.emission;
		const glm::vec3 view = glm::normalize(ray_start - hit.position);
		glm::vec3 reflectVec = glm::normalize(2.f * glm::dot(view, hit.normal) * hit.normal - view);
		//loop through all lights
		for (int i = 0; i < numused; i++) {
			//cout << numused << endl;
			Light curlight = lightList[i];
			glm::vec3 lightDir;
			float distance = 0.f;
			float atten = 1.f;

			if (curlight.lighttype == POINT) {
				distance = glm::length(curlight.position - hit.position);
				lightDir = glm::normalize(curlight.position - hit.position);
				atten = 1.0f / (distance * distance * quadratic + attenuation + distance * linear);
			}
			else if (curlight.lighttype == DIRECTIONAL) {
				lightDir = glm::normalize(curlight.position);
			}
			
			//create shadow by checking if hit point to light source is blocked by object
			Ray hitToSource(hit.position, lightDir);
			Intersection blockPoint;
			bool blocked = Intersect(hitToSource, scene, &blockPoint);
			bool notdark;

			if (curlight.lighttype == POINT){
				notdark = (distance <= blockPoint.hitDistance);
			}
			else {
				notdark = (!blocked);
			}

			//return lighting if this ray should be a shadow
			if (notdark) {
				//calculate lighting with phong shading
				glm::vec3 halfvec = glm::normalize(view + lightDir);
				float nDotL = glm::dot(hit.normal, lightDir);
				float nDotH = glm::dot(hit.normal, halfvec);
				glm::vec3 diffu = hit.object.diffuse * glm::max(nDotL, 0.0f);
				glm::vec3 spec = hit.object.specular * glm::pow( glm::max(nDotH, 0.0f), shininess);

				lighting +=  curlight.color * atten * (diffu + spec);
			}

			//recursive tracing for reflection
			if (hit.recursionDepth + 1 < maxdepth) {

				//glm::vec3 trureflect = glm::reflect(view, hit.normal);
				Ray reflected = Ray(hit.position, reflectVec);
				Intersection reflectedHit;
				//if hit
				if (Intersect(reflected, scene, &reflectedHit)) {
					reflectedHit.recursionDepth = hit.recursionDepth + 1;
					lighting += hit.object.specular * FindColor(reflectedHit, scene, hit.position);
				}
			}


		}
	}
	//return lighting;
	return glm::clamp(lighting, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	
}

Image* Raytrace(const Camera& cam, const Scene& scene, int w, int h) {
	Image* image = new Image(w, h);
	ProgressBar pg;
	pg.start(h);

	#pragma omp parallel for default(none) schedule(static) shared(pg)
	for (int i = 0; i < h; i++) {
		pg.update(i);
		for (int j = 0; j < w; j++) {
			Ray ray = RayThruPixel(cam, i, j);
			Intersection hit;
			if (Intersect(ray, scene, &hit)) {
				image->pixmat[i][j] = FindColor(hit, scene, eye);
			}
			else {
				image->pixmat[i][j] = glm::vec3{ 0.f,0.f,0.f };
			}
		}
	}
	return image;
}

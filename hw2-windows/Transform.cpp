// Transform.cpp: implementation of the Transform class.

#include "Transform.h"

// Helper rotation function.
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	float radian = glm::radians(degrees);

	mat3 id(1);
	mat3 x(0, axis[2], -axis[1], -axis[2], 0, axis[0], axis[1], -axis[0], 0);

	mat3 rotated = cos(radian) * id + (1 - cos(radian))*(glm::outerProduct(axis, axis)) +
		sin(radian) * x;

	return rotated;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	up = glm::normalize(up);

	eye = eye * rotate(degrees, -up);
	up = up * rotate(degrees, -up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	up = glm::normalize(up);

	vec3 normeye = glm::normalize(eye);
	vec3 a = glm::cross(up, normeye);

	eye = eye * rotate(degrees, a);
	up = up * rotate(degrees, a);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	vec3 c = glm::normalize(eye);

	vec3 b = glm::normalize(up);

	vec3 a = glm::cross(b, c);

	mat4 build(a[0], b[0], c[0], 0, a[1], b[1], c[1], 0, a[2], b[2], c[2], 0,
		dot(-a, eye), dot(-b, eye), dot(-c, eye), 1);

	return build;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	fovy = fovy/2;
    mat4 ret((1/(aspect*tanf(fovy))), 0,0,0, 0,(1/(tanf(fovy))),0,0, 
		0,0,-((zFar+zNear)/(zFar-zNear)),-1, 0,0,-((2*zFar*zNear)/(zFar-zNear)),0);

	mat4 correct = glm::perspective(fovy, aspect, zNear, zFar);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret(sx,0,0,0,0,sy,0,0,0,0,sz,0,0,0,0,1);

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret(1,0,0,0,0,1,0,0,0,0,1,0,tx,ty,tz,1);

    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}

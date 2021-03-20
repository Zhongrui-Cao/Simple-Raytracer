// This is the basic include file for the global variables in the program.  

#ifndef VARIABLES_H
#define VARIABLES_H

#include "Myclasses.h"
#include <cmath>
#include <glm/glm.hpp>
#include <string>

//extern int amount; // The amount of rotation for each arrow press
extern glm::vec3 eye; // The vector coordinates of the eye 
extern glm::vec3 up;  // The vector coordinates of the up 

extern glm::vec3 eyeinit ;
extern glm::vec3 upinit ;
extern glm::vec3 center ;
extern int amountinit;
extern int w, h ;
extern float fovy ;

extern float sx, sy ; // the scale in x and y 
extern float tx, ty ; // the translation in x and y

// Materials (read from file) 
extern glm::vec3 ambient;
extern glm::vec3 diffuse;
extern glm::vec3 specular;
extern glm::vec3 emission;
extern float shininess;
extern float attenuation;
extern float linear;
extern float quadratic;

// For multiple objects, read from a file.  
extern const int maxobjects; 
extern int numobjects ;
extern const int numLights;
extern int numused;                     // How many lights are used 

extern Camera camera;
extern Scene scene;

//max number of recursion depth
extern int maxdepth;
extern int maxverts;
extern std::vector<glm::vec3> vertices;

extern std::vector<Light> lightList;

extern std::string outputfile;

extern int numtest;

extern int progress;
#endif
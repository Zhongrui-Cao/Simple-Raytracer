#include "variables.h"


Camera camera;
Scene scene;
std::vector<glm::vec3> vertices;
std::vector<Light> lightList(numLights);
int numobjects = 0;
const int numLights = 10;
const int maxobjects = 10;
int numused = 0;
int w = 600, h = 400;

glm::vec3 eye(0, 0, 0), up(0, 0, 0);
glm::vec3 eyeinit(0, 0, 0), upinit(0, 0, 0);
glm::vec3 center(0.0, 0.0, 0.0);
float fovy;
glm::vec3 ambient(0);
glm::vec3 diffuse(0);
glm::vec3 specular(0);
glm::vec3 emission(0);
float shininess = 0;
int maxdepth = 5;
float attenuation = 1;
float linear = 0;
float quadratic = 0;
float sx, sy; // the scale in x and y 
float tx, ty; // the translation in x and y
int maxverts = 100000;
std::string outputfile = "test.jpg";
int numtest;
int progress = 0;

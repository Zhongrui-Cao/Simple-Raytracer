// The functions readvals and readfile do basic parsing. 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace std;
#include "variables.h" 
#include "readfile.h"

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, float* values) 
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename) 
{
    string str, cmd; 
    ifstream in;
	int vertexCount = 0;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                int i; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Material Commands 
				if (cmd == "directional") {
                    validinput = readvals(s, 6, values); // colors 
                    if (validinput) {
						lightList[numused] = Light(DIRECTIONAL, values[0], 
							values[1], values[2], values[3], values[4], values[5]);
						numused++;
                    }
                }
				else if (cmd == "point") {
					validinput = readvals(s, 6, values); // colors 
					if (validinput) {
						lightList[numused] = Light(POINT, values[0],
							values[1], values[2], values[3], values[4], values[5]);
						numused++;
					}
				}
				else if (cmd == "attenuation") {
					validinput = readvals(s, 3, values); // colors 
					if (validinput) {
						attenuation = values[0];
						linear = values[1];
						quadratic = values[2];
					}
				}
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i]; 
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i]; 
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i]; 
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i]; 
                        }
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1]; 
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
					
						eye = vec3(values[0], values[1], values[2]);
						center  = vec3(values[3], values[4], values[5]);
						vec3 uptemp = vec3(values[6], values[7], values[8]);
						vec3 view = eye - center;
						up  = Transform::upvector(uptemp, view);
						fovy = glm::radians(values[9]);
						camera = Camera(eye, fovy, w, h);
                    }
                } else if (cmd == "maxdepth") {
					validinput = readvals(s, 1, values);
					if (validinput)
						maxdepth = values[0];
				} else if (cmd == "maxverts") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						maxverts = values[0];
						vertices = std::vector<glm::vec3>(maxverts);
					}
				} else if (cmd == "vertex") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						/*
						for (i = 0; i < 3; i++) {
							vertices[vertexCount][i] = values[i];
						}*/
						vertices[vertexCount].x = values[0];
						vertices[vertexCount].y = values[1];
						vertices[vertexCount].z = values[2];
						vertexCount++;
					}
				}

				else if (cmd == "tri") {
					validinput = readvals(s, 3, values);
					if (validinput) {

						Object object;

						glm::mat4 &m = transfstack.top();
						object.v0 = glm::vec3 (m * glm::vec4(vertices[values[0]], 1));
						object.v1 = glm::vec3 (m * glm::vec4(vertices[values[1]], 1));
						object.v2 = glm::vec3 (m * glm::vec4(vertices[values[2]], 1));

						object.ambient = ambient;
						object.diffuse = diffuse;
						object.specular = specular;
						object.emission = emission;
						object.shininess = shininess;
						object.type = std::string("tri");
						scene.objects.push_back(object);
						numobjects++;
					}
				}

				else if (cmd == "sphere") {
					validinput = readvals(s, 4, values);
					if (validinput) {

						Object object;

						object.radius = values[3];
						object.v0 = vec3(values[0], values[1], values[2]);
						object.transform = transfstack.top();

						object.ambient = ambient;
						object.diffuse = diffuse;
						object.specular = specular;
						object.emission = emission;
						object.shininess = shininess;
						object.type = std::string("sphere");
						scene.objects.push_back(object);
						numobjects++;
					}
				}

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
						rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
						rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {
						vec3 axis = vec3(values[0], values[1], values[2]);
						glm::normalize(axis);
						mat3 rot = Transform::rotate(values[3], axis);
						rightmultiply(mat4(rot), transfstack);

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }
				else if (cmd == "output") {
				}
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Transform.h"
#include <FreeImage.h>
#include <glm/ext.hpp>
#include "variables.h" 
#include "readfile.h" 
#include "Raytracer.h"

int main(int argc, char* argv[]) {

  FreeImage_Initialise();
  char filename[20];
  int c;
  std::cout << "Please enter the scene file name: ";
  std::cin >> filename;

  readfile(filename);

  Image* image = Raytrace(camera, scene, w, h);

  BYTE* pixels = new BYTE[w * h * 3];

  for (int i = 0; i < h; ++i)
  {
	  for (int j = 0; j < w; ++j)
	  {
		  int ir = (int)(255.f * (image->pixmat[i][j].x));
		  int ig = (int)(255.f * (image->pixmat[i][j].y));
		  int ib = (int)(255.f * (image->pixmat[i][j].z));
		  pixels[i*w * 3 + 3 * j + 2] = ir;
		  pixels[i*w * 3 + 3 * j + 1] = ig;
		  pixels[i*w * 3 + 3 * j + 0] = ib;
	  }
  }

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
  std::cout << "\nSaving screenshot to: " << outputfile.c_str() << "\n";
  FreeImage_Save(FIF_PNG, img, outputfile.c_str(), 0);
  FreeImage_DeInitialise();
  //return 0;
  std::cout << "Render finished, ";
  system("pause");
}

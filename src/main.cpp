// g++ --std=c++11 -w *.cpp -lGLEW -lGLUT -lGLFW -framework OpenGL  -o run && ./run

#ifndef MAIN_CPP
#define MAIN_CPP

#include "ModelVisualizer.hpp"
#include "Colors.cpp"

int main(int argc, char const *argv[])
{
  ModelVisualizer mv(colors::brown);
  mv.addModel("../models/horse.ply", colors::red, 1); // with specular
  mv.addModel("../models/cow.ply", colors::blue, 0); // only diffuse
  mv.visualize();
  
  return 0;
}

#endif // MAIN_CPP
// g++ -w *.cpp -lGLEW -lGLUT -lGLFW -framework OpenGL  -o tut

#ifndef MAIN_CPP
#define MAIN_CPP

#include "ModelVisualizer.hpp"

int main(int argc, char const *argv[])
{
  ModelVisualizer mv = ModelVisualizer(colors::blue, "../shaders/StandardShading.vertexshader", "../shaders/StandardShading.fragmentshader");

  // mv.addModel("../models/horse.ply");
  mv.addModel("../models/cow.ply");
  mv.visualize();
  
  return 0;
}

#endif // MAIN_CPP
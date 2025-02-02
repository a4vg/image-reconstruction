// g++ --std=c++11 -w *.cpp -lGLEW -lGLUT -lGLFW -framework OpenGL  -o run && ./run

#ifndef MAIN_CPP
#define MAIN_CPP

#include "Reconstructor.hpp"
#include "Colors.cpp"

int main(int argc, char const *argv[])
{
  std::string filePattern = "data/pacient-";
  std::string ext = "BMP";

  Reconstructor reconstructor(filePattern, ext, 1, 40, colors::darkBlue);
  reconstructor.visualize(colors::white);

  return 0;
}

#endif // MAIN_CPP

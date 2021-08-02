// g++ --std=c++11 -w *.cpp -I ../include/ -I ../lib/ -lX11 -o run && ./run

// -lGLEW -lGLUT -lGLFW -framework OpenGL

#ifndef MAIN_CPP
#define MAIN_CPP

#include "Image.h"

int main() {
  Image image("../data/pacient-1.BMP");
  image.binarize();
  image.contours();

  image.show();
  image.save("../output/pacient.BMP");
  return 0;
}

#endif // MAIN_CPP

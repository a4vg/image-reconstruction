#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "CImg/CImg.h"
#include "Point.h"

using namespace cimg_library;

class Image {
private:
  CImg<unsigned char> img;
  CImgDisplay display;
public:
  Image(const char* filename);
  ~Image(){};

  void binarize(float threshold=.5);
  void contours();
  void save(const char* filename);
  void show();
};

Image::Image(const char* filename) {
  img.load(filename);
  img.channel(0);
}

void Image::binarize(float threshold) {
  for (int r=0; r<img.height(); ++r)
    for (int c=0; c<img.width(); ++c) {
      img(c,r,0,0) = (img(c,r,0,0) > threshold*255)*255;
      
      // Get rid of white borders and right scale bar
      if (!c || !r || c>=img.width()-30 || r>=img.height()-3)
        img(c,r,0,0) = 0;
    }
}

void Image::contours(){
  CImg<unsigned char> img2(img, "xyzc", 0);

  for (int r=0; r<img.height(); ++r){
    Point lastOn(0,0);
    bool firstInRow = true;
    for (int c=0; c<img.width(); ++c) {
      if (img(c,r,0,0)) {
        if (firstInRow) {
          firstInRow = false;
          img2(c,r,0,0) = 255;
        }
        lastOn.set(c,r);
      }
    }
    img2(lastOn.x,lastOn.y,0,0) = 255;
  }

  for (int c=0; c<img.height(); ++c){
    Point lastOn(0,0);
    bool firstInColumn = true;
    for (int r=0; r<img.width(); ++r) {
      if (img(c,r,0,0)) {
        if (firstInColumn) {
          firstInColumn = false;
          img2(c,r,0,0) = 255;
        }
        lastOn.set(c,r);
      }
    }
    img2(lastOn.x,lastOn.y,0,0) = 255;
  }

  img.assign(img2);
}

void Image::save(const char* filename) {
  img.save(filename);
}

void Image::show() {
  img.display(display); 
  while (!display.is_closed()) display.wait();
}

#endif // IMAGE_CPP

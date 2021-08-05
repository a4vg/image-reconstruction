#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include "CImg/CImg.h"
#include "Point.hpp"

using namespace cimg_library;

class Image {
private:
  CImg<unsigned char> rawImg;
  CImgDisplay display;
public:
  Image(const char* filename);
  Image(std::string filename) : Image((filename).c_str() ) {};
  ~Image(){};

  void binarize(float threshold=.5);
  void contours();
  void save(const char* filename);
  void save(std::string filename) { save((filename).c_str()); };
  void show();

  friend class PointCloudModel;
};

Image::Image(const char* filename) {
  rawImg.load(filename);
  rawImg.channel(0);
}

void Image::binarize(float threshold) {
  for (int r=0; r<rawImg.height(); ++r)
    for (int c=0; c<rawImg.width(); ++c) {
      rawImg(c,r,0,0) = (rawImg(c,r,0,0) > threshold*255)*255;
      
      // Get rid of white borders and right scale bar
      if (!c || !r || c<30 || c>=rawImg.width()-30 || r>=rawImg.height()-3)
        rawImg(c,r,0,0) = 0;
    }
}

void Image::contours(){
  CImg<unsigned char> img2(rawImg, "xyzc", 0);

  for (int r=0; r<rawImg.height(); ++r){
    Point lastOn(0,0);
    bool firstInRow = true;
    for (int c=0; c<rawImg.width(); ++c) {
      if (rawImg(c,r,0,0)) {
        if (firstInRow) {
          firstInRow = false;
          img2(c,r,0,0) = 255;
        }
        lastOn.set(c,r);
      }
    }
    img2(lastOn.x,lastOn.y,0,0) = 255;
  }

  for (int c=0; c<rawImg.height(); ++c){
    Point lastOn(0,0);
    bool firstInColumn = true;
    for (int r=0; r<rawImg.width(); ++r) {
      if (rawImg(c,r,0,0)) {
        if (firstInColumn) {
          firstInColumn = false;
          img2(c,r,0,0) = 255;
        }
        lastOn.set(c,r);
      }
    }
    img2(lastOn.x,lastOn.y,0,0) = 255;
  }

  rawImg.assign(img2);
}

void Image::save(const char* filename) {
  rawImg.save(filename);
}

void Image::show() {
  rawImg.display(display); 
  while (!display.is_closed()) display.wait();
}

#endif // IMAGE_H

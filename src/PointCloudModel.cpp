#ifndef POINTCLOUDMODEL_CPP
#define POINTCLOUDMODEL_CPP

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "PointCloudModel.hpp"
#include "Image.hpp"

PointCloudModel::PointCloudModel(glm::vec3 color)
: color(color) {}

PointCloudModel::~PointCloudModel()
{
  glDeleteBuffers(1, &vertexbuffer);
}

void PointCloudModel::loadImage(std::string infile, std::string outfile) {

  Image img(infile);
  img.binarize();
  img.contours();

  for (int r=0; r<img.rawImg.height(); ++r)
    for (int c=0; c<img.rawImg.width(); ++c) {
      if (img.rawImg(c,r,0,0)) {
        float halfWidth = img.rawImg.width()/2;
        float halfHeight = img.rawImg.height()/2;
        vertices.push_back(glm::vec3((c-halfWidth)/(halfWidth*1.6), layerN, (r-halfHeight)/(halfHeight*1.6)));
      }
    }

  img.save(outfile);
}

void PointCloudModel::computeMatrices() {
  // PointCloudModel matrix: Rotation
  ModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(modelAngY), glm::vec3(1.0f, 0.0f, 0.0f));
  ModelMatrix = glm::rotate(ModelMatrix, glm::radians(modelAngX), glm::vec3(0.0f, 1.0f, 0.0f));
  ModelMatrix = glm::rotate(ModelMatrix, glm::radians(modelAngZ), glm::vec3(0.0f, 0.0f, 1.0f));

  ModelMatrix = glm::translate(ModelMatrix, glm::vec3(modelPosX, modelPosY, 0.0f));

  // PointCloudModel matrix: Scale
  ModelMatrix = glm::scale(ModelMatrix, glm::vec3(modelScale));
}

#endif // POINTCLOUDMODEL_CPP

#ifndef POINTCLOUDMODEL_HPP
#define POINTCLOUDMODEL_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Colors.cpp"

class PointCloudModel
{
private:
  std::vector<glm::vec3> vertices;

  GLuint vertexbuffer;
  GLuint ModelMatrixID;
  glm::mat4 ModelMatrix;
  glm::mat4 MVP;
  
  float modelAngX = 0.0f;
  float modelAngY = 0.0f;
  float modelAngZ = 0.0f;

  float modelPosX = 0.0f;
  float modelPosY = 0.0f;

  float modelScale = 1.0f;

  glm::vec3 color;

  void computeMatrices();

  friend class ModelVisualizer;
  
public:
  float layerN = 0;

  PointCloudModel(){};
  PointCloudModel(glm::vec3 color);
  ~PointCloudModel();

  void loadImage(std::string infile, std::string outfile);
};

#endif // POINTCLOUDMODEL_HPP
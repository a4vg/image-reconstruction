#ifndef RECONSTRUCTOR_HPP
#define RECONSTRUCTOR_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>

#include "ModelVisualizer.hpp" // Includes PointCloudModel.hpp

class Reconstructor
{
private:
	ModelVisualizer visualizer;
  PointCloudModel pointCloudModel;
  
public:
  Reconstructor(std::string filePattern, std::string ext, int from, int to);
  ~Reconstructor(){};

  static void process(std::string filePattern, std::string ext, int from, int to);

  void initOpenGL();
  void initMatrices();
  void visualize(glm::vec3 bgcolor);
};

#endif // RECONSTRUCTOR_HPP
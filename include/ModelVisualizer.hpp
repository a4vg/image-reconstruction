#ifndef MODELVISUALIZER_HPP
#define MODELVISUALIZER_HPP

#include <vector>
#include <string>
#include <memory>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "PointCloudModel.hpp"

class ModelVisualizer
{
private:
  GLFWwindow* window;

  PointCloudModel* model;
  
  GLuint VertexArrayID;
  GLuint programID;
  GLuint ViewMatrixID;
  GLuint MatrixID;
  GLuint BaseColorID;
  
  glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

  // Initial position : on +Z
  glm::vec3 position = glm::vec3( 0, 0, 5 ); 
  // Initial horizontal angle : toward -Z
  float horizontalAngle = 3.14f;
  // Initial vertical angle : none
  float verticalAngle = 0.0f;
  // Initial Field of View
  float initialFoV = 45.0f;

  float speed = 3.0f; // 3 units / second
  double lastPressZ = glfwGetTime();
  double lastTime = glfwGetTime();

  void computeMatricesFromInputs();
  void instructions();
public:
  void init(glm::vec3 bgcolor, PointCloudModel& model);
  void visualize();

  ModelVisualizer(){};
  ~ModelVisualizer();
};

#endif // MODELVISUALIZER_HPP
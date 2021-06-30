#include <vector>
#include <memory>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Model.hpp"

class ModelVisualizer
{
private:
  GLFWwindow* window;
  GLuint VertexArrayID;
  GLuint programID;
  GLuint ViewMatrixID;
  GLuint MatrixID;
  GLuint LightID;
  GLuint BaseColorID;

  unsigned int cur_model;
  std::vector<std::shared_ptr<Model> > models;
  // Model model;

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
public:
  void visualize();
  void computeMatricesFromInputs();
  void addModel(const char* ply_file, glm::vec3 color);
  
  ModelVisualizer();
  ~ModelVisualizer();
};
#ifndef MODELVISUALIZER_HPP
#define MODELVISUALIZER_HPP

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Model.hpp"
#include "Colors.cpp"

class ModelVisualizer {
private:
  GLFWwindow* window;
  GLuint VertexArrayID;
  GLuint programId;
  GLuint LightID;

  const float speed = 3.0f; // 3 units / second
  const float initialFoV = 45.0f; // Initial Field of View

  glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
  GLuint ViewMatrixID;

  int curModelIdx = -1;
  std::vector<Model> models;

  void init();
  void computeInputs();
  void setShader(const char* vertex_shader, const char* fragment_shader);
public:
  ModelVisualizer(Color bgcolor, const char* vertex_shader, const char* fragment_shader);
  ~ModelVisualizer();

  void addModel(const char* ply_file);
  void visualize();
};

#endif // MODELVISUALIZER_HPP
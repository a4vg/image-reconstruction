#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Colors.cpp"

class Model
{
private:
  static int count;

  std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

  GLuint vertexbuffer;
  GLuint normalbuffer;

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
  int specular;

  void computeMatrices();

  friend class ModelVisualizer;
public:
  Model(glm::vec3 color, int spec);
  ~Model();

  bool loadPLY(const char * path);
};
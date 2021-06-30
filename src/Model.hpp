#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

class Model {
private:
  static const float horizontalAngle = 3.14f; // Initial horizontal angle : toward -Z
  static const float verticalAngle = 0.0f; // Initial vertical angle : none
  GLuint programId;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;

  glm::vec3 position;
  float angX;
  float angY;
  float angZ;

  float scale;

	glm::mat4 modelMatrix;
	glm::mat4 MVP;
  GLuint MatrixID;
  GLuint ModelMatrixID;

  GLuint vertexbuffer;
  GLuint normalbuffer;

  bool loadPLY(const char* path);
  void computeMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix);
  void draw();

  friend class ModelVisualizer;
public:
  Model(const char* ply_file, GLuint programId);
  ~Model();
};

#endif // MODEL_HPP
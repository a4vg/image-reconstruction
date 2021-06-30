#ifndef MODEL_CPP
#define MODEL_CPP

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Model.hpp"

Model::Model(const char* ply_file, GLuint programId)
: programId(programId), position(glm::vec3( 0, 0, 5 )), angX(0.0f), angY(0.0f), angZ(0.0f), scale(1.0f){
  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(programId, "MVP");
  ModelMatrixID = glGetUniformLocation(programId, "M");

	printf("Program id: %d\n", programId);

  loadPLY(ply_file);
}

Model::~Model() {
  glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
}

bool Model::loadPLY(const char * path) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

  int n_vertices;
  int n_faces;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Could not open file!\n");
		getchar();
		return false;
	}

  char line[128];

  /**
   * HEADER
   **/
  fgets(line, 128, file);
  strncmp("element vertex", line, strlen("element vertex"));
  // element vertex n_vertices
  while (strncmp("element vertex", line, strlen("element vertex")) != 0) fgets(line, 128, file);
  sscanf(line, "%*s %*s %d", &n_vertices);
  // element faces n_faces
  while (strncmp("element face", line, strlen("element face")) != 0) fgets(line, 128, file);
  sscanf(line, "%*s %*s %d", &n_faces);
  // go to end of header
  while (strncmp("end_header", line, strlen("end_header")) != 0) fgets(line, 128, file);

  /**
   * VERTICES
   **/
  while (n_vertices--) {
    glm::vec3 vertex, normal;
    fscanf(file, "%f %f %f %f %f %f", &vertex.x, &vertex.y, &vertex.z, &normal.x, &normal.y, &normal.z);
    temp_vertices.push_back(vertex);
    temp_normals.push_back(normal);
  }

  /**
   * FACES
   **/
  while (n_faces--) {
    unsigned int index[3];
    fscanf(file, "%*d %d %d %d", &index[0], &index[1], &index[2]);
    indices.push_back(index[0]);
    indices.push_back(index[1]);
    indices.push_back(index[2]);
  }

  fclose(file);

	// For each vertex of each triangle
	for( unsigned int i=0; i<indices.size(); i++ ){
		// Get the indices of its attributes
		unsigned int index = indices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ index ];
		glm::vec3 normal = temp_normals[ index ];
		
		// Put the attributes in buffers
		vertices.push_back(vertex);
		normals.push_back(normal);
	}

  //  Load into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	return true;
}

void Model::computeMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix) {
  // Model matrix: Rotation
	modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angY), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angX), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angZ), glm::vec3(0.0f, 0.0f, 1.0f));

	// Model matrix: Scale
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
	MVP = projectionMatrix * viewMatrix * modelMatrix;

	for (int f=0; f<4; ++f) {
			for (int c=0; c<4; ++c)
				printf("%f ", MVP[f][c]);
			printf("\n");}
			printf("\n");

  // Send our transformation to the currently bound shader
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
}

void Model::draw() {
  // 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

#endif // MODEL_CPP
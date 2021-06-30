#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "plyloader.hpp"

bool loadPLY(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec3> & out_normals
){
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
		out_vertices.push_back(vertex);
		out_normals.push_back(normal);
	}
	return true;
}

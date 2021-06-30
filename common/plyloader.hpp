#ifndef PLYLOADER_HPP
#define PLYLOADER_HPP

bool loadPLY(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec3> & out_normals
);
#endif
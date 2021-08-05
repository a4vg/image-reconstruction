#version 330 core

layout(location = 0) in vec3 vertices;

uniform mat4 MVP;

// in vec4 color;

// out vec4 vertColor;

void main() {
  gl_Position = MVP * vec4(vertices,1);
	gl_PointSize = 30.0;
    
  // vertColor = color;
}
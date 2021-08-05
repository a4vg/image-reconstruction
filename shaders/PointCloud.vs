#version 330 core

layout(location = 0) in vec3 vertices;

uniform mat4 MVP;
uniform vec3 color;

out vec4 pointColor;

void main() {
  gl_Position = MVP * vec4(vertices,1);
	gl_PointSize = 30.0;
    
  pointColor = vec4(color, 1.0f);
}
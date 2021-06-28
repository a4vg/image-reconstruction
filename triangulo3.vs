// triangulo.vs

// attribute vec4 myVertex; 
in vec4 myVertex;

uniform vec4 diffuseColor;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// varying vec3 vertexColor;
varying vec4 vVaryingColor;

void main (void) {
	vVaryingColor.xyz = diffuseColor.xyz;
	vVaryingColor.a = 1.0;

	//gl_Position = mvpMatrix * vec4(myVertex, 1.0);
	gl_Position = projection * view * model * myVertex;
	//gl_Position = gl_Position / gl_Position.w;

}
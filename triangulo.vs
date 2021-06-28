// triangulo.vs

// attribute vec4 myVertex; 
in vec4 myVertex;

uniform vec4 diffuseColor;
uniform mat4 mvpMatrix;

// varying vec3 vertexColor;
varying vec4 vVaryingColor;

void main (void) {
	vVaryingColor.xyz = diffuseColor.xyz;
	vVaryingColor.a = 1.0;

	//gl_Position = mvpMatrix * vec4(myVertex, 1.0);
	gl_Position = mvpMatrix * myVertex;

}
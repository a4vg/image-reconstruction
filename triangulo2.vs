// triangulo2.vs

// attribute vec4 myVertex; 
in vec4 myVertex;
// attribute vec3 myNormal;
in vec3 myNormal;

uniform vec4 diffuseColor;
uniform vec3 vLightPosition;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

// varying vec3 vertexColor;
varying vec4 vVaryingColor;

void main (void) {
	// vertexColor = 0.01 * myVertex.xyz;
	// vertexColor = myVertex.xyz;
	// gl_Position = myVertex; 
	//gl_BackColor = vec4(1.0, 0.0, 0.0, 1.0);

	vec3 vEyeNormal = normalMatrix * myNormal;
	// vec4 nvertex = vec4(myVertex, 1.0);
	vec4 vPosition4 = mvMatrix * myVertex;
	vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

	vec3 vLightDir = normalize(vLightPosition - vPosition3);
	float diff = max(0.0, dot(vEyeNormal, vLightDir));
	vVaryingColor.xyz = diff * diffuseColor.xyz;
	// vVaryingColor.xyz = diffuseColor.xyz;
	vVaryingColor.a = 1.0;

	gl_Position = mvpMatrix * myVertex;

}
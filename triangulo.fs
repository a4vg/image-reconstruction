// triangulo2.fs
//
// Sets fragment color to red.

// varying vec3 vertexColor;
// uniform vec4 vColorValue;
varying vec4 vVaryingColor;

void main(void) {
   // gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
   // gl_FragColor = vec4(vertexColor, 1.0);
   // gl_FragColor = gl_Color;
   gl_FragColor = vVaryingColor;
}

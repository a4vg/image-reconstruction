#ifndef MODELVISUALIZER_CPP
#define MODELVISUALIZER_CPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Colors.cpp"
#include "Shader.hpp"

using namespace std;

#include "ModelVisualizer.hpp"

ModelVisualizer::ModelVisualizer(Color bgcolor, const char* vertex_shader, const char* fragment_shader) {
  init();
	// Set bg color
  glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, 0.0f);

	programId = Shader::load(vertex_shader, fragment_shader);

	ViewMatrixID = glGetUniformLocation(programId, "V");

	glUseProgram(programId);
	LightID = glGetUniformLocation(programId, "LightPosition_worldspace");

	printf("Program id: %d\n", programId);
}

ModelVisualizer::~ModelVisualizer() {
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	printf("Program id: %d\n", programId);
}

void ModelVisualizer::addModel(const char* ply_file) {
	curModelIdx = 0;
	models.push_back(Model(ply_file, programId));
	printf("Program id: %d\n", programId);
};

void ModelVisualizer::init() {
  // Initialise GLFW
	if(!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Model Visualizer", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

  // VAO
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
}

void ModelVisualizer::computeInputs(){
  // glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(models[curModelIdx].verticalAngle) * sin(models[curModelIdx].horizontalAngle), 
		sin(models[curModelIdx].verticalAngle),
		cos(models[curModelIdx].verticalAngle) * cos(models[curModelIdx].horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(models[curModelIdx].horizontalAngle - 3.14f/2.0f), 
		0,
		cos(models[curModelIdx].horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	/**
	 * Move
	 **/
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS){
		models[curModelIdx].position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		models[curModelIdx].position -= direction * deltaTime * speed;
	}
	// Move right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		models[curModelIdx].position += right * deltaTime * speed;
	}
	// Move left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		models[curModelIdx].position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		models[curModelIdx].position += up * deltaTime * speed;
	}
	// // Move down
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		models[curModelIdx].position -= up * deltaTime * speed;
	}

	/**
	 * Rotate
	 **/
	// Rotate up
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		models[curModelIdx].angY -= 20.0*deltaTime*speed;
	}
	// Rotate down
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		models[curModelIdx].angY += 20.0*deltaTime*speed;
	}
	// Rotate left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		models[curModelIdx].angX -= 20.0*deltaTime*speed;
	}
	// Rotate right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		models[curModelIdx].angX += 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
		models[curModelIdx].angZ -= 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
		models[curModelIdx].angZ += 20.0*deltaTime*speed;
	}

	/**
	 * Scale
	 **/
	// Increase
	if (glfwGetKey( window, GLFW_KEY_O  ) == GLFW_PRESS){
		models[curModelIdx].scale += 0.5 * deltaTime * speed;
	}
	// Decrease
	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
		models[curModelIdx].scale -= 0.5 * deltaTime * speed;
	}

	/**
	 * Change current model
	 **/
	if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		curModelIdx = (curModelIdx+1)%models.size();
		printf("Down\n");
	}


	float FoV = initialFoV;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	viewMatrix = glm::lookAt(
								models[curModelIdx].position,           // Camera is here
								models[curModelIdx].position+direction, // and looks here : at the same models[curModelIdx].position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void ModelVisualizer::visualize() {
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programId);
		printf("Program id: %d\n", programId);
		computeInputs();

		for (auto &model: models) model.computeMatrices(projectionMatrix, viewMatrix);
		
		// Compute light
		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Send transformation to the currently bound shader
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		for (auto &model: models) model.draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
}

#endif // MODELVISUALIZER_CPP
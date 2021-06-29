// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getModelMatrix(){
	return ModelMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float modelAngX = 0.0f;
float modelAngY = 0.0f;
float modelAngZ = 0.0f;

float modelScale = 1.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	/**
	 * Move
	 **/
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Move right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Move left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += up * deltaTime * speed;
	}
	// // Move down
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= up * deltaTime * speed;
	}

	/**
	 * Rotate
	 **/
	// Rotate up
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		modelAngY -= 20.0*deltaTime*speed;
	}
	// Rotate down
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		modelAngY += 20.0*deltaTime*speed;
	}
	// Rotate left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		modelAngX -= 20.0*deltaTime*speed;
	}
	// Rotate right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		modelAngX += 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
		modelAngZ -= 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
		modelAngZ += 20.0*deltaTime*speed;
	}

	/**
	 * Scale
	 **/
	// Increase
	if (glfwGetKey( window, GLFW_KEY_O  ) == GLFW_PRESS){
		modelScale += 0.5 * deltaTime * speed;
	}
	// Decrease
	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
		modelScale -= 0.5 * deltaTime * speed;
	}


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );


	// Model matrix: Rotation
	ModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(modelAngY), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(modelAngX), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(modelAngZ), glm::vec3(0.0f, 0.0f, 1.0f));

	// Model matrix: Scale
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(modelScale));

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
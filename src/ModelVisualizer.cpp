#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Shader.hpp"
#include "ModelVisualizer.hpp"

ModelVisualizer::ModelVisualizer(glm::vec3 bgcolor)
{
	instructions();

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context`
	window = glfwCreateWindow( 1024, 768, "Tutorial 08 - Basic Shading", NULL, NULL);
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

	//GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Dark blue background
	glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], 0.0f);

	// Create and compile our GLSL program from the shaders 
	programID = Shader::load( "../shaders/StandardShading.vertexshader", "../shaders/StandardShading.fragmentshader" );
	glUseProgram(programID);

	// Get a handle for our "MVP" uniform
	ViewMatrixID = glGetUniformLocation(programID, "V");
	MatrixID = glGetUniformLocation(programID, "MVP");

	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	BaseColorID = glGetUniformLocation(programID, "base_color");
	WithSpecularID = glGetUniformLocation(programID, "with_specular");
}

void ModelVisualizer::addModel(const char* ply_file, glm::vec3 color, int specular) {
	cur_model = 0;
	std::shared_ptr<Model> model = std::make_shared<Model>(color, specular);
	model->ModelMatrixID = glGetUniformLocation(programID, "M");

  // Read our .ply file
  model->loadPLY(ply_file);

  // Load it into a VBO
  glGenBuffers(1, &model->vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, model->vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), &model->vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &model->normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, model->normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, model->normals.size() * sizeof(glm::vec3), &model->normals[0], GL_STATIC_DRAW);
	models.push_back(model);
}

ModelVisualizer::~ModelVisualizer()
{

  // Cleanup VBO and shader
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


void ModelVisualizer::visualize() {
	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard input
		computeMatricesFromInputs();

		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		for (auto& model: models) {
			model->computeMatrices();
			model->MVP = ProjectionMatrix * ViewMatrix * model->ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &model->MVP[0][0]);
			glUniformMatrix4fv(model->ModelMatrixID, 1, GL_FALSE, &model->ModelMatrix[0][0]);
			glUniform3fv(BaseColorID, 1, &model->color[0]);
			glUniform1d(WithSpecularID, model->specular);

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, model->vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 2nd attribute buffer : normals
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, model->normalbuffer);
			glVertexAttribPointer(
				1,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, model->vertices.size() );

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
}

void ModelVisualizer::computeMatricesFromInputs() {
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
	 * Move camera
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
	 * Move model
	 **/
	// Move up
	if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS){
		models[cur_model]->modelPosY +=  deltaTime * speed;
	}
	// Move down
	if (glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS){
		models[cur_model]->modelPosY -= deltaTime * speed;
	}
	// Move right
	if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
		models[cur_model]->modelPosX += deltaTime * speed;
	}
	// Move left
	if (glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS){
		models[cur_model]->modelPosX -= deltaTime * speed;
	}

	/**
	 * Rotate
	 **/
	// Rotate up
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		models[cur_model]->modelAngY -= 20.0*deltaTime*speed;
	}
	// Rotate down
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		models[cur_model]->modelAngY += 20.0*deltaTime*speed;
	}
	// Rotate left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		models[cur_model]->modelAngX -= 20.0*deltaTime*speed;
	}
	// Rotate right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		models[cur_model]->modelAngX += 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
		models[cur_model]->modelAngZ -= 20.0*deltaTime*speed;
	}
	// Rotate depth
	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
		models[cur_model]->modelAngZ += 20.0*deltaTime*speed;
	}

	/**
	 * Scale
	 **/
	// Increase
	if (glfwGetKey( window, GLFW_KEY_O  ) == GLFW_PRESS){
		models[cur_model]->modelScale += 0.5 * deltaTime * speed;
	}
	// Decrease
	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
		models[cur_model]->modelScale -= 0.5 * deltaTime * speed;
	}

	/**
	 * Change current model
	 **/
	if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS){
		if (currentTime-lastPressZ>0.5) {
			cur_model = (cur_model+1)%models.size();
			printf("Next: %d\n", cur_model);
			lastPressZ = currentTime;
		}
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



	lastTime = currentTime;
}

void ModelVisualizer::instructions() {
	printf("\n************ Instructions ************");
	printf("\n*  Move camera: arrows               *");
	printf("\n*  Move model: [j][i][k][l]          *");
	printf("\n*  Rotate model: [a][w][s][d]        *");
	printf("\n*  Scale model: [n][m]               *");
	printf("\n*  Switch to next model: [z]         *");
	printf("\n*  Quit: [Esc]                       *");
	printf("\n**************************************\n\n");
}
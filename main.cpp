// g++ -w main.cpp matriz.cpp -lGLEW -lGLUT -lGLFW -framework OpenGL  -o tut

#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
// #  include <OpenGL/glext.h>
#  include <GLFW/glfw3.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") // Compiler directive to include the GLEW library.
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Matriz.h"
#include "Normal.h"

GLFWwindow* window;
float cameraX, cameraY, cameraZ;
float lposx, lposy, lposz;
float cubeLocX, cubeLocY, cubeLocZ;
glm::mat4 projection, model, view;
int width, height;
float aspect;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate
static float Scale = 0.1;
bool imprimir_una_vez = true;

GLuint p1_id;
GLint p1_mat_mv_id, p1_mat_mvp_id, p1_color_difuso_id;
GLint p1_projection_id, p1_view_id, p1_model_id;
GLint p1_vertex_id = 0, p1_normal_id = 1;
GLint p1_light_pos_id, p1_light_color_id;
GLint p1_object_color_id;

unsigned int VBO[2], prismaVAO[2];

float prisma_normales[12];
static float prisma_vertices[] = {
  0, 0, 0,
  4, 0, 0,
  2, 0, 4,
  2, 4, 2,
};
static unsigned int prisma_indices[] = {
  0, 1, 2,
  0, 1, 3,
  1, 3, 2,
  0, 2, 3
};
static float normalMatrix[] = {
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f
};

// NEW: Function to read a shader file.
char* readShader(const std::string &aShaderFile) {
  FILE* filePointer = fopen(aShaderFile.c_str(), "rb");
  if (filePointer == NULL) {
    std::cout << "Couldn not open file" << "\n";
    exit(1);
  }
  char* content;
  long numVal;

  fseek(filePointer, 0L, SEEK_END);
  numVal = ftell(filePointer);
  fseek(filePointer, 0L, SEEK_SET);
  content = (char*) malloc((numVal+1) * sizeof(char));
  fread(content, 1, numVal, filePointer);
  content[numVal] = '\0';
  fclose(filePointer);
  return content;
}

static void Error(const std::string &message) {
  std::cout << message;
}

/* Compila shader */
static void CompileShader (GLuint id) {
  GLint status;
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLint len;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
    char* message = (char*) malloc(len*sizeof(char));
    glGetShaderInfoLog(id, len, nullptr, message);
    Error(message);
    free(message);
  }
}

/* Link−edita shader */
static void LinkProgram (GLuint id) {
  GLint status;
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (!status) {
    GLint len;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
    char* message = (char*) malloc(len*sizeof(char));
    glGetProgramInfoLog(id, len, nullptr, message);
    Error(message);
    free(message);
  }
}

/* Cria programa de shader */
static void CreateShaderProgram(const std::string &vertexShaderFile, const std::string &fragmentShaderFile, GLuint &p_id) {
  char*	vertexShader   = readShader(vertexShaderFile);
  char*	fragmentShader = readShader(fragmentShaderFile);

  /* vertex shader */
  GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
  if (v_id == 0)
    Error("Could not create vertex shader object");

  glShaderSource(v_id, 1, (const char**) &vertexShader, nullptr);
  CompileShader(v_id);

  /* fragment shader */
  GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
  if (f_id == 0)
    Error("Could not create fragment shader object");

  glShaderSource(f_id, 1, (const char**) &fragmentShader, nullptr);
  CompileShader(f_id);

  /* program */
  p_id = glCreateProgram();
  if (p_id == 0)
    Error("Could not create program object");
  glAttachShader(p_id, v_id);
  glAttachShader(p_id, f_id);
  LinkProgram(p_id);
}


// Initialization routine.
void setup() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
  lposx = 10.0f; lposy = 10.0f; lposz = 10.0f;
  cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective

  glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.
  glEnable(GL_DEPTH_TEST);
  // CreateShaderProgram("./2.1.basic_lighting.vs", "./2.1.basic_lighting.fs", p1_id);
  CreateShaderProgram("./2.1.basic_lighting_vertex.vs", "./2.1.basic_lighting_vertex.fs", p1_id);
  glBindAttribLocation(p1_id, p1_vertex_id, "aPos");
  glBindAttribLocation(p1_id, p1_normal_id, "aNormal");
  p1_projection_id	= glGetUniformLocation(p1_id, "projection");
  p1_view_id  		= glGetUniformLocation(p1_id, "view");
  p1_model_id	    	= glGetUniformLocation(p1_id, "model");
  p1_light_pos_id 	= glGetUniformLocation(p1_id, "lightPos");
  p1_light_color_id 	= glGetUniformLocation(p1_id, "lightColor");
  p1_object_color_id 	= glGetUniformLocation(p1_id, "objectColor");


  // calculando normales
  Matriz tmp;
  //float prisma_normales[12];
  float tn[12];  // temporal normales

  int cpn = 0;
  float v1[3], v2[3], normal[3];
  for (int j = 0; j < 12; j+=3) {
    v1[0] = prisma_vertices[3*prisma_indices[j+1] + 0] - prisma_vertices[3*prisma_indices[j] + 0];
    v1[1] = prisma_vertices[3*prisma_indices[j+1] + 1] - prisma_vertices[3*prisma_indices[j] + 1];
    v1[2] = prisma_vertices[3*prisma_indices[j+1] + 2] - prisma_vertices[3*prisma_indices[j] + 2];

    v2[0] = prisma_vertices[3*prisma_indices[j+2] + 0] - prisma_vertices[3*prisma_indices[j+1] + 0];
    v2[1] = prisma_vertices[3*prisma_indices[j+2] + 1] - prisma_vertices[3*prisma_indices[j+1] + 1];
    v2[2] = prisma_vertices[3*prisma_indices[j+2] + 2] - prisma_vertices[3*prisma_indices[j+1] + 2];

    tmp.crossProduct(v1, v2, normal);
    tmp.normalize(normal);
    tn[cpn++] = normal[0];
    tn[cpn++] = normal[1];
    tn[cpn++] = normal[2];
  }

  for (int i=0; i < 12; i++) {
    std::cout << tn[i] << " ";
  }
  Normal n1, n2, n3, np;
  cpn = 0;
  n1.x = tn[0]; n1.y = tn[1]; n1.z = tn[2];
  n2.x = tn[3]; n2.y = tn[4]; n2.z = tn[5];
  n3.x = tn[9]; n3.y = tn[10]; n3.z = tn[11];
  np = n1 + n2 + n3;

  prisma_normales[cpn++] = np.x;
  prisma_normales[cpn++] = np.y;
  prisma_normales[cpn++] = np.z;


  n1.x = tn[0]; n1.y = tn[1]; n1.z = tn[2];
  n2.x = tn[3]; n2.y = tn[4]; n2.z = tn[5];
  n3.x = tn[6]; n3.y = tn[7]; n3.z = tn[8];
  np = n1 + n2 + n3;

  prisma_normales[cpn++] = np.x;
  prisma_normales[cpn++] = np.y;
  prisma_normales[cpn++] = np.z;


  n1.x = tn[0]; n1.y = tn[1]; n1.z = tn[2];
  n2.x = tn[6]; n2.y = tn[7]; n2.z = tn[8];
  n3.x = tn[9]; n3.y = tn[10]; n3.z = tn[11];
  np = n1 + n2 + n3;

  prisma_normales[cpn++] = np.x;
  prisma_normales[cpn++] = np.y;
  prisma_normales[cpn++] = np.z;


  n1.x = tn[3]; n1.y = tn[4]; n1.z = tn[5];
  n2.x = tn[6]; n2.y = tn[7]; n2.z = tn[8];
  n3.x = tn[9]; n3.y = tn[10]; n3.z = tn[11];
  np = n1 + n2 + n3;

  prisma_normales[cpn++] = np.x;
  prisma_normales[cpn++] = np.y;
  prisma_normales[cpn++] = np.z;

  //-0.604119 -0.43771 0.665921 0.604119 -0.43771 0.665921 -0.193858 -0.451337 0.871042 -0.252185 0.0579244 0.965944
  std::cout << "\nNormales\n";
  for (int i=0; i < 12; i++) {
    std::cout << prisma_normales[i] << " ";
  }


  glGenVertexArrays(2, prismaVAO);
  glGenBuffers(2, VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(prisma_vertices), prisma_vertices, GL_STATIC_DRAW);

  glBindVertexArray(prismaVAO[0]);
  glVertexAttribPointer(p1_vertex_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(p1_vertex_id);
  glBindVertexArray(prismaVAO[1]);
  glVertexAttribPointer(p1_normal_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(p1_normal_id);

}

// Drawing routine.
void drawScene() {
  int vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  aspect = (float)vp[2] / (float)vp[3];
  projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f); // 1.0472 radians = 60 degrees

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(glm::mat4(1.0f), glm::radians(Yangle), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(Scale));



  GLboolean transpose = GL_FALSE;

  if (imprimir_una_vez) {
    std::cout << "\nViewport " << vp[0] << " " << vp[1] << " " << vp[2] << " " << vp[3];
    float pos[] = {4, 0, 0};
    imprimir_una_vez = false;
    glm::vec4 v1(4,0,0, 1);
    glm::vec4 v11 = projection * view * model * v1;
    std::cout << "\nv {" << v11[0] << ", "<< v11[1]<< ", " << v11[2]<< ", " << v11[3] << "}\n";
  }

  glUseProgram(p1_id);

  //glVertexAttribPointer(p1_vertex_id, 3, GL_FLOAT, GL_FALSE, 0, prisma_vertices);
  //glVertexAttribPointer(p1_normal_id, 3, GL_FLOAT, GL_FALSE, 0, prisma_normales);
  //glEnableVertexAttribArray(p1_vertex_id);
  //glEnableVertexAttribArray(p1_normal_id);

  glBindVertexArray(prismaVAO[0]);

  glUniformMatrix4fv(p1_projection_id, 1, transpose, glm::value_ptr(projection));
  glUniformMatrix4fv(p1_view_id, 1, transpose, glm::value_ptr(view));
  glUniformMatrix4fv(p1_model_id, 1, transpose, glm::value_ptr(model));
  static float aColorDifuso_prisma[] = {1.0f, 0.0f, 0.0f};
  static float aLightPos[] = {lposx, lposy, lposz};
  static float aLightColor[] = {1.0f, 1.0f, 1.0f};
  glUniform3fv(p1_object_color_id, 1, aColorDifuso_prisma);
  glUniform3fv(p1_light_pos_id, 1, aLightPos);
  glUniform3fv(p1_light_color_id, 1, aLightColor);

  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (const void *) prisma_indices);
  glDisableVertexAttribArray(p1_vertex_id); //
  glDisableVertexAttribArray(p1_normal_id);

  // glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

// Keyboard input processing routine.
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action != GLFW_PRESS) return;
  switch (key) {
    case 27:
      exit(0);
    case GLFW_KEY_M:
      cameraZ--;
      std::cout << "Z " << cameraZ << "\n";
      break;
    case GLFW_KEY_N:
      cameraZ++;
      std::cout << "Z " << cameraZ << "\n";
      break;
    case GLFW_KEY_A:
      Xangle += 1.0;
      if (Xangle > 360.0) Xangle -= 360.0;
      break;
    case GLFW_KEY_D:
      Xangle -= 1.0;
      if (Xangle < 0.0) Xangle += 360.0;
      break;
    case GLFW_KEY_W :
      Yangle += 5.0;
      if (Yangle > 360.0) Yangle -= 360.0;
      std::cout << "Y " << Yangle << "\n";
      break;
    case GLFW_KEY_S:
      Yangle -= 5.0;
      if (Yangle < 0.0) Yangle += 360.0;
      break;
    case GLFW_KEY_E:
      Zangle += 5.0;
      if (Zangle > 360.0) Zangle -= 360.0;
      break;
    case GLFW_KEY_R:
      Zangle -= 5.0;
      if (Zangle < 0.0) Zangle += 360.0;
      break;
    case GLFW_KEY_P:
      Scale += 0.01;
      std::cout << Scale << "\n";
      break;
    case  GLFW_KEY_O:
      Scale -= 0.01;
      if (Scale == 0.0) Scale = 0.01;
      std::cout << Scale << "\n";
      break;
    case GLFW_KEY_DOWN:
      lposy -= 0.5;
      std::cout << "LPY" << lposy << "\n";
      break;
    case GLFW_KEY_UP:
      lposy += 0.5;
      std::cout << "LPY" << lposy << "\n";
      break;
    case GLFW_KEY_L:
      imprimir_una_vez = true;
      break;
    default:
      break;
  }
}


void printInteraction(void) {
  std::cout << "Interaction:\n";
  std::cout << "Scale: [p][o]\n";
  std::cout << "Rotate:\n\tX axis: [a][d]\n\tY axis: [w][s]\n\tZ axis: [e][r]\n";
  std::cout << "Move:\n\tX axis: [←][→]\n\tY axis: [↑][↓]\n\tZ axis: [m][n]\n";
}

// Main routine.
int main(int argc, char **argv) {
  printInteraction();

  // Initialise GLFW
  if (!glfwInit()) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
	window = glfwCreateWindow(500, 500, "Visualizando modelo", NULL, NULL);
	if (window == nullptr) {
    std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

  // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		glfwTerminate();
		return -1;
	}

  // glutInit(&argc, argv);

  // // glutInitContextVersion(4, 3);
  // glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  // glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  // glutInitWindowSize(500, 500);
  // glutInitWindowPosition(100, 100);
  // glutCreateWindow("Visualizando modelo");
  // glutDisplayFunc(drawScene);
  // glutReshapeFunc(resize);
  // glutKeyboardFunc(keyboard);

  
  glfwSetKeyCallback(window, keyboard);
  glfwSetFramebufferSizeCallback(window, resize);
  resize(window, 500, 500);

  // Display OpenGL version information
  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  printf("OpenGL Version: %d.%d\n", major, minor);
  printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  setup();

  while (!glfwWindowShouldClose(window)) {
    drawScene();
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // glutMainLoop();
  return 0;
}


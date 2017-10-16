// Include the glmain.cpp.o module definitions.
#include "glmain.h"

// Include symbols from submodules.
#include "jpegld.h"
#include "glshader.h"

// Other trivial headers.
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

// The keyboard handle function.
// And its related controls.
float rotationX = 0; float rotationY = -65.0;
int textureMode = 1;
void handleKeyboard(GLFWwindow* window, int key, 
	int scancode, int action, int mods);

int main(int argc, char** argv) {
	int jpegWidth, jpegHeight, jpegComponent;
	std::vector<GLubyte> jpegData;

	// Before initializing the OpenGL environments.
	// We should load essential resources for displaying.
	loadJpeg(argc, argv, jpegWidth, jpegHeight, 
		jpegComponent, jpegData);

	// Initialization of GLFW framework and window.
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(
		jpegWidth * 2, jpegHeight * 2, 
		"CG2017_Lab01", nullptr, nullptr);
	glfwSetKeyCallback(window, handleKeyboard);
	glfwMakeContextCurrent(window);

	// Initialization of GL3W corearb loader library.
	gl3wInit();
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Shading Language Version: %s\n", 
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Load Jpeg textures.
	GLuint jpegTexture;
	glGenTextures(1, &jpegTexture);
	glBindTexture(GL_TEXTURE_2D, jpegTexture);
	GLenum jpegFormat = jpegComponent == 3? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, jpegFormat, 
		jpegWidth, jpegHeight,
		jpegFormat, GL_UNSIGNED_BYTE, &jpegData[0]);
	jpegData.resize(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(glGetError() != GL_NO_ERROR) error(argc, argv,
		"Error while uploading GL texture!", 5);

	// Create the shader program now.
	GLint texCoordIn, texSlot, texMode;
	GLuint shaderProgram = createLabProgram(
		argc, argv, texCoordIn, texSlot, texMode);
	glUseProgram(shaderProgram);

	

	// Setup rendering pre-states.
	// Reset model space to (0, 0, w, h).
	glViewport(0, 0, jpegWidth * 2, jpegHeight * 2);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 3.0);

	// Set color buffer clear color to black.
	glClearColor(0, 0, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glUniform1i(texSlot, 0); // Slot bound to texture #0.
	//glUseProgram(0);

	// Loop the window update frame.
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glTranslated(0, 0, -0.8);
		glRotated(rotationX, 0.0, -1.0, 0);
		glRotated(rotationY, 1.0, 0.0, 0);
		glScaled(2.0/jpegWidth, -2.0/jpegHeight, 0);
		glTranslated(-jpegWidth/2.0, -jpegHeight/2.0, 0);

		// Select the proper texture mode.
		glUniform1i(texMode, textureMode);

		// Render the checker board image.
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glColor4d(1, 1, 1, 1);
		glBegin(GL_QUADS);
			//specify texCoordIn
			glVertexAttrib2d(texCoordIn, 0, 0);
			glTexCoord2d(0, 0);
			glVertex2d(0, 0);
			glVertexAttrib2d(texCoordIn, 1, 0);
			glTexCoord2d(1, 0);
			glVertex2d(jpegWidth, 0);
			glVertexAttrib2d(texCoordIn, 1, 1);
			glTexCoord2d(1, 1);
			glVertex2d(jpegWidth, jpegHeight);
			glVertexAttrib2d(texCoordIn, 0, 1);
			glTexCoord2d(0, 1);
			glVertex2d(0, jpegHeight);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroy the GLFW framework and window.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// Implementation for the error handler.
// Will display the error and exit process directly.
void error(int argc, char** argv, 
	const char* message, int errorStatus) {

	// Print out the error message.
	fprintf(stderr, "Error:");
	for(int i = 0; i < argc; ++ i)
		fprintf(stderr, " %s", argv[i]);
	fprintf(stderr, ":%s", message);

	// Exit process directly.
	exit(errorStatus);
}

// Implementation for the key handler.
void handleKeyboard(GLFWwindow* window, int key, 
	int scancode, int action, int mods) {
	
	// Default axis biases and settings.
	float deltaX = 5.f;
	float deltaY = 5.f;
	int numPadInput = 0;
	char const* modeName = "";

	if(action == GLFW_PRESS || action == GLFW_REPEAT) switch(key) {
		// Controls related to Y axis.
		case GLFW_KEY_DOWN: 
			deltaY = -deltaY;
		case GLFW_KEY_UP:
			rotationY += deltaY;
		break;

		// Controls related to X axis.
		case GLFW_KEY_LEFT:
			deltaX = -deltaX;
		case GLFW_KEY_RIGHT:
			rotationX += deltaX;
		break;

		// Switch texture mode according to keypad.
		case GLFW_KEY_1: numPadInput = 1; 
			modeName = "nearest"; break;
		case GLFW_KEY_2: numPadInput = 2; 
			modeName = "bilinear"; break;
		case GLFW_KEY_3: numPadInput = 3; 
			modeName = "bilinear/mipmap"; break;
		case GLFW_KEY_4: numPadInput = 4; 
			modeName = "trilinear/mipmap"; break;
		case GLFW_KEY_5: numPadInput = 5; 
			modeName = "2x anisotropic"; break;
		case GLFW_KEY_6: numPadInput = 6; 
			modeName = "4x anisotropic"; break;
		case GLFW_KEY_7: numPadInput = 7; 
			modeName = "8x anisotropic"; break;
		case GLFW_KEY_8: numPadInput = 8; 
			modeName = "16x anisotropic"; break;

		default:
			break;
	}

	// Switch and print texture mode.
	if(numPadInput > 0) {
		textureMode = numPadInput;
		printf("Using texture mode #%d: %s\n", 
			textureMode, modeName);
	}
}

#pragma once

/**
 * For object files that are not glmain.cpp.o,
 * they should use the symbols from OpenGL API
 * directly, while the main class should not.
 */
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl3w.h>

// Display the error message and directly 
// terminate the entire process. Used to 
// capture and report illegal state.
void error(int argc, char** argv, 
	const char* errorMsg, int returnStatus);

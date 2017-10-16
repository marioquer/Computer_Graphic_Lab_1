#pragma once
#include "glmain.h"

// Defines the shader creation and loading function.
// This function will create a proper GLSL program
// for rendering.
GLuint createLabProgram(int argc, char** argv,
	GLint& texCoordIn, GLint& texSlot, GLint& texMode);

#pragma once
#include "glmain.h"
#include <vector>

void loadJpeg(int argc, char** argv,
	int& jpegWidth, int& jpegHeight, 
	int& jpegComponent, 
	std::vector<GLubyte>& jpegData);

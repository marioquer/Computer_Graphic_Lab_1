#include "jpegld.h"

// JPEG loading libraries (IJG).
#include <cstdio>
#include <cstring>
#include <jpeglib.h> 
#include <setjmp.h>

// Process global control blocks and functions.
jmp_buf jpegJumpBuffer;
METHODDEF(void) jpegErrorExit(j_common_ptr jpegInfo) {
	longjmp(jpegJumpBuffer, 1);
}

// Implementation for the jpeg loading library.
// Will load the jpeg into memory for updating 
// and displaying and image processing.
void loadJpeg(int argc, char** argv, 
	int& jpegWidth, int& jpegHeight, 
	int& jpegComponent, 
	std::vector<GLubyte>& jpegData) {

	// Read and stat the image at resource/subject.JPG.
	// First we should try to open the file.
	FILE* jpegFile = fopen("resource/subject.JPG", "r");
	if(jpegFile == NULL) error(argc, argv,
		"The subject.JPG file is not found!" 
		"Make sure the subject jpeg file is "
		"located in the resource directory", 1);
	
	// File exists, then we should try to load the file now.
	// Initialize jpeg loading environments.
	struct jpeg_decompress_struct jpegStruct;
	struct jpeg_error_mgr jpegError;

	// Setup error handler for Jpeg library.
	jpegStruct.err = jpeg_std_error(&jpegError);
	jpegError.error_exit = jpegErrorExit;
	if(setjmp(jpegJumpBuffer)) error(argc, argv, 
		"Error while reading jpeg file."
		"Make sure the jpeg file is not "
		"corrupted or not readable.", 2);

	// Initialize the decompress struct.
	jpeg_create_decompress(&jpegStruct);
	jpeg_stdio_src(&jpegStruct, jpegFile);
	
	// Read the jpeg header and prepare for input now.
	jpeg_read_header(&jpegStruct, 1);
	jpeg_start_decompress(&jpegStruct);

	jpegWidth = jpegStruct.output_width;
	jpegHeight = jpegStruct.output_height;
	jpegComponent = jpegStruct.output_components;
	jpegData.resize(jpegWidth * jpegHeight * jpegComponent);
	int jpegRowStride = jpegWidth * jpegComponent;
	JSAMPARRAY jpegBuffer = (jpegStruct.mem -> alloc_sarray)
		((j_common_ptr) &jpegStruct, JPOOL_IMAGE, jpegRowStride, 1);

	// Read into the input info now.
	int jpegOffset = 0;
	while(jpegStruct.output_scanline < jpegHeight) {
		jpeg_read_scanlines(&jpegStruct, jpegBuffer, 1);
		memcpy(&jpegData[jpegOffset], jpegBuffer[0], jpegRowStride);
		jpegOffset += jpegRowStride;
	}

	// Clean up now.
	jpeg_finish_decompress(&jpegStruct);
	jpeg_destroy_decompress(&jpegStruct);
	fclose(jpegFile);
}

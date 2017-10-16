#include "glshader.h"
#include <cstdio>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

GLuint createShaderProg(const char* vertexPath, const char* fragmentPath);
GLuint compileShader(GLenum type, const char* shaderCode, char*& log);


// Implementation for the lab's shader program.
GLuint createLabProgram(int argc, char** argv,
	GLint& texCoordIn, GLint& texSlot, GLint& texMode) {

	GLuint shaderProgram = "shader/vShader.glsl","shader/fShader.glsl");

	//set index for following updating
	texCoordIn = glGetAttribLocation(shaderProgram, "texCoordIn");
	texSlot = glGetUniformLocation(shaderProgram, "texSlot");
	texMode = glGetUniformLocation(shaderProgram, "texMode");

    	cout << texCoordIn << texSlot << texMode << endl;

	return shaderProgram;
}

GLuint createShaderProg(const char* vertexPath, const char* fragmentPath){
        // retrieve shader source code from filePath
		char* log = NULL;
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }
        catch (ifstream::failure e){
            cout << "[ERROR] SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        //compile the shader source
        GLuint vShader = compileShader(GL_VERTEX_SHADER, vShaderCode, log),
        	fShader = compileShader(GL_FRAGMENT_SHADER, fShaderCode, log);

        //compile status
        if(!vShader || !fShader){
        	return 0;
        }

        //link shader
        GLuint progID = glCreateProgram();
        glAttachShader(progID, vShader);
        glAttachShader(progID, fShader);
        glLinkProgram(progID);

        //check link status
        int success;
        char infoLog[512];
        glGetProgramiv(progID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(progID, 512, NULL, infoLog);
            cout << "[ERROR] SHADER::PROGRAM_LINKING_FAILED: " << infoLog << endl;
        }

        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return progID;
}

GLuint compileShader(GLenum type, const char* shaderCode, char*& log){
	//create shader object
    GLuint shader = glCreateShader(type);
    if( !shaderCode )
        return 0;
    const char* ptrShaderSource = shaderCode;
    //bind source to obj
    glShaderSource(shader,1,&ptrShaderSource,NULL);
    //compile obj
    glCompileShader(shader);
    GLint status = 0;
    //compile status
    glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
    if( !status )
    {
        GLint length;
        //read log length
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
        log = (GLchar*)malloc(length);
        //read log
        glGetShaderInfoLog(shader,length,&length,log);
        cout << "[ERROR] SHADER::COMPILE_FAILED: " << log << endl;
        //delete obj
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
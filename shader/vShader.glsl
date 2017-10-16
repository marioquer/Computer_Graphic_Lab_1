#version 130
attribute vec2 texCoordIn;
varying vec2 tempCoordIn;

void main()
{
    tempCoordIn = texCoordIn;
    gl_Position = ftransform();
}
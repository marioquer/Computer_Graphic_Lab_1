#version 130


varying vec2 tempCoordIn;
uniform int texMode;
uniform sampler2D texSlot;

out vec4 FragColor;

void main()
{
	switch(texMode){
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
	}
    FragColor = vec4(0,0.4f,0, 1.0f);
}
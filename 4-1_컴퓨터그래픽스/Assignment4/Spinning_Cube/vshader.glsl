#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color_out;
uniform mat4 rotation;

void main()
{
	gl_Position = rotation * vPosition ;
	color_out = vColor;
}
#version 150

in vec4 vPosition;
in vec3 vColor;
out vec4 color_out;

void main(void)
{
	gl_Position = vPosition;
	color_out = vec4(vColor, 1.0);
}
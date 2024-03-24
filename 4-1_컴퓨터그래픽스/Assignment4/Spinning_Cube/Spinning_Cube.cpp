#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "cube.h"
//#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET( offset )  ((GLvoid*) (offset))

void mouse(int btn, int state, int x, int y);
void spinCube();
GLuint InitShader(const char* vShaderFile, const char* fShaderFile);
static char* readShaderSource(const char* shaderFile);

GLfloat theta[3] = { 0.0, 0.0, 0.0 };
int axis = 0;
GLuint program;

void Init() {
    colorcube();
    
    //코딩
	//glMatrixMode(GL_MODELVIEW);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	//GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc2 = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

    glEnable(GL_DEPTH_TEST);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //코딩
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glm::mat4 ctm(1.0f);
	
	//ctm = glm::rotate(ctm, theta[0], glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(ctm, theta[1], glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(ctm, theta[2], glm::vec3(0.0f, 0.0f, 1.0f));
	ctm = glm::rotate(ctm, glm::radians(theta[0]), glm::vec3(-1.0f, 0.0f, 0.0f));
	ctm = glm::rotate(ctm, glm::radians(theta[1]), glm::vec3(0.0f, -1.0f, 0.0f));
	ctm = glm::rotate(ctm, glm::radians(theta[2]), glm::vec3(0.0f, 0.0f, 1.0f));

	GLint matrix_loc = glGetUniformLocation(program, "rotation");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, glm::value_ptr(ctm));


    glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    glutSwapBuffers();
}
void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        axis = 0;
    if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        axis = 1;
    if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        axis = 2;
}
void spinCube()
{
    theta[axis] += 0.5;
    if (theta[axis] > 360.0) theta[axis] -= 360.0;
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("colorcube");

	glewInit();

    Init();

    glutDisplayFunc(display);
    glutIdleFunc(spinCube);
    glutMouseFunc(mouse);

    glutMainLoop();
}
// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}
// Create a GLSL program object from vertex and fragment shader files
GLuint
InitShader(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader {
		const char* filename;
		GLenum       type;
		GLchar* source;
	}  shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 2; ++i) {
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL) {
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	/* use program object */
	glUseProgram(program);

	return program;
}
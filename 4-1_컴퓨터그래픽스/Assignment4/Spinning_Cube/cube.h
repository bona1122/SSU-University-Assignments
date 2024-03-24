#pragma once
#include <glm/vec4.hpp>

const int NumVertices = 36;	// (6 faces)(2 triangles/face)(3 vertices/triangle)

glm::vec4 points[NumVertices];
glm::vec4 colors[NumVertices];

//Verticex of a unit cube centered at origin, sides alighed with axes
glm::vec4 vertices[8] = {
	glm::vec4(-0.5, -0.5,  0.5, 1.0),
	glm::vec4(-0.5,  0.5,  0.5, 1.0),
	glm::vec4( 0.5,  0.5,  0.5, 1.0),
	glm::vec4( 0.5, -0.5,  0.5, 1.0),
	glm::vec4(-0.5, -0.5, -0.5, 1.0),
	glm::vec4(-0.5,  0.5, -0.5, 1.0),
	glm::vec4( 0.5,  0.5, -0.5, 1.0),
	glm::vec4( 0.5, -0.5, -0.5, 1.0)
};

// RGBA colors
glm::vec4 vertex_color[8] = {
	glm::vec4(0.0, 0.0,  0.0, 1.0),	//black
	glm::vec4(1.0, 0.0,  0.0, 1.0),	//red
	glm::vec4(1.0, 1.0,  0.0, 1.0),	//yelllow
	glm::vec4(0.0, 1.0,  0.0, 1.0),	//green
	glm::vec4(0.0, 0.0,  1.0, 1.0),	//blue
	glm::vec4(1.0, 0.0,  1.0, 1.0),	//magenta
	glm::vec4(1.0, 1.0,  1.0, 1.0),	//white
	glm::vec4(0.0, 1.0,  1.0, 1.0)	//cyan
};

//quad generates two triangles for each face and assigns colors
// to the vertices
int Index = 0;
void quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_color[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_color[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_color[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_color[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_color[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_color[d]; points[Index] = vertices[d]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>

void mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.5f, 0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.5f, 0.5f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutCreateWindow("simple");
    glutDisplayFunc(mydisplay);

    glutMainLoop();

    return EXIT_SUCCESS;
}

#include <iostream>
#include <GL/freeglut.h>
#include <Math.h>

#define PI 3.14159265f

GLint windowWidth = 512;
GLint windowHeight = 512;

GLfloat ballRadius = 0.5f;
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat colors[][3] = {
    {1.0, 0.0, 0.0},    //red
    {0.0, 1.0, 0.0},    //green
    {0.0, 0.0, 1.0}     //blue
};
int colorIdx = 0;

void display();
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void wheel(int wheel, int direction, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Moving Ball");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseWheelFunc(wheel);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();

    return EXIT_SUCCESS;
}
void motion(int x, int y) {
    ballX = x * (2.0f / 512.0f) - 1;
    ballY = -(y * (2.0f / 512.0f) - 1);
    glutPostRedisplay();
    printf("(%d, %d) -> (%f, %f)\n", x, y, ballX, ballY);
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ballX = x * (2.0f / 512.0f) - 1;
        ballY = -(y * (2.0f / 512.0f) - 1);
        printf("(%d, %d) -> (%f, %f)\n", x, y, ballX, ballY);
        glutPostRedisplay();
    }
}
void wheel(int wheel, int direction, int x, int y) {
    if (direction > 0) {
        ballRadius += 0.1f;
    }
    else {
        ballRadius -= 0.1f;
    }
    glutPostRedisplay();
}
void special(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_F1:
        colorIdx = (colorIdx + 1) % 3;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
void keyboard(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(ballX, ballY, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glColor3fv(colors[colorIdx]);
        glVertex2f(0.0f, 0.0f); //Center or circle
        int numSegments = 100;
        GLfloat angle;
        for (int i = 0; i <= numSegments; i++) {
            angle = i * 2.0f * PI / numSegments;
            glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
        }
    glEnd();

    glutSwapBuffers();
}
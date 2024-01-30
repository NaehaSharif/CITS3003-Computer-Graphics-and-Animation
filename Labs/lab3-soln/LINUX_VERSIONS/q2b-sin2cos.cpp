/* Sierpinski gasket with a vertex array with rotation */

#include "Angel.h"

using namespace std;

GLuint sinAngle, cosAngle;

const int NumPoints = 400000;

//----------------------------------------------------------------------------

vec2 points[NumPoints];

void init(void)
{
    // Specify the vertices for a triangle
    vec2 vertices[3] = {
        vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
    };

    points[0] = vec2(0.25, 0.50);

    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 3;   // pick a vertex at random

        points[i] = (points[i - 1] + vertices[j]) / 2.0;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint program = InitShader("q2b-2sincos-v.glsl", "q2b-2sincos-f.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    sinAngle = glGetUniformLocation(program, "sinAngle");
    cosAngle = glGetUniformLocation(program, "cosAngle");

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(sinAngle, sin(0.002 * glutGet(GLUT_ELAPSED_TIME)));
    glUniform1f(cosAngle, cos(0.001 * glutGet(GLUT_ELAPSED_TIME)));
    glDrawArrays(GL_POINTS, 0, NumPoints);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033:
        exit(EXIT_SUCCESS);
        break;
    }
}

//----------------------------------------------------------------------------

void idle(void)
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Lab 3: ");

    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

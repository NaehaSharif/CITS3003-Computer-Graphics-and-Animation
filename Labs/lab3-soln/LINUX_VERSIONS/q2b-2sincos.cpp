/* Sierpinski gasket with a vertex array and rotation */

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
    vec3 red = vec3(1.0, 0.0, 0.0);

    // Select an arbitrary initial point inside of the triangle
    points[0] = vec2(0.25, 0.50);    

    // compute and store N-1 new points
    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        // and the previous point
        points[i] = (points[i - 1] + vertices[j]) / 2.0;
    }

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // First, we create an empty buffer of the size we need by passing
    //  a NULL pointer for the data values
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("q2b-2sincos-v.glsl", "q2b-2sincos-f.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    sinAngle = glGetUniformLocation(program, "sinAngle");
    cosAngle = glGetUniformLocation(program, "cosAngle");

    // glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(sinAngle, 0.5*sin(0.001 * glutGet(GLUT_ELAPSED_TIME)));
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
    glutCreateWindow("Lab 3");

    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

/** Sierpinski gasket in 3D (tetrahedron)
 *
 *  In this program, we demonstrate how to create 2 buffer objects and how to
 *  deal with 3D points.
 *
 *  Du Huynh
 *  August 2015
 */

#include "Angel.h"

using namespace std;

GLuint transformID;

const int NumPoints = 4000000; // adjust this number yourself

//----------------------------------------------------------------------------

vec3 points[NumPoints];
vec3 colors[NumPoints];

void init(void)
{
    // Specify the vertices for a triangular prism
    vec3 vertices[4] = {
        vec3(-0.8, -0.8, -0.8), vec3(0.0, 0.8, -0.8),
        vec3(0.8, -0.8, -0.8), vec3(0.0, -0.8, 0.8)
    };

    points[0] = vec3(0.25, 0.50, 0.0);
    colors[0] = vec3(0.0,0.0,0.0);

    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 4;   // pick a vertex at random

        points[i] = (points[i - 1] + vertices[j]) / 2.0;
        colors[i][2] = colors[i-1][1];
        colors[i][1] = colors[i-1][0];
        colors[i][0] = 0.16 * j + 0.5 * colors[i-1][2];
    }

    GLuint program = InitShader("q3xyzTransform.glsl", "fpassthru.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize two buffer objects
    GLuint buffer[2];
    glGenBuffers(2, buffer);

    // Specify the binding point for buffer[0], the first buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    // Create a bufer large enough to store the 'points' array defined above.
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    // Likewise, do the same for the vertex color attribute. This time, we use
    // buffer[1], the second buffer object.

    // Specify the binding point for buffer[1], the second buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    // Create a bufer large enough to store the 'colors' array defined above.
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Initialize the vertex color attribute from the vertex shader    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    transformID = glGetUniformLocation(program, "transform");

    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    
    mat3 mTransform = mat3(cos(angle), 0.0, -sin(angle),
                           0.0,        1.0,         0.0,
                           sin(angle), 0.0, cos(angle));
    
    glUniformMatrix3fv(transformID, 1, GL_FALSE, mTransform);
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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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

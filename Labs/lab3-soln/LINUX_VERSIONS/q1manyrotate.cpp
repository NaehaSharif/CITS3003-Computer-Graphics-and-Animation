/* Sierpinski gasket with a vertex array and a colour array plus rotation */

#include "Angel.h"

using namespace std;

GLint timeParam;

//const int NumPoints = 40000000;
const int NumPoints = 10000000; // adjust the number of points yourself

//----------------------------------------------------------------------------

vec2 points[NumPoints];
vec3 colors[NumPoints];

void init(void)
{
    // Specify the vertices for a triangle
    vec2 vertices[3] = {
        vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = vec2(0.25, 0.50);
    colors[0] = vec3(1.0, 0.0, 0.0);


    // compute and store N-1 new points
    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = (points[i - 1] + vertices[j]) / 2.0;
        colors[i] = colors[i-1];
        vec3 newcolor = vec3(0.0, 0.0, 0.0);
        newcolor[j] = 1.0;
                
        colors[i] = colors[i-1] * 0.8 + newcolor*0.2;
    }

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
                  NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

    GLuint program = InitShader("q1vrotate2d.glsl", "fpassthru.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)));

    timeParam = glGetUniformLocation(program, "time");

    // glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    //No depth buffer is required as we deal with 2D here.
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));
    glDrawArrays(GL_POINTS, 0, NumPoints);
    
    // we need the line below rather than glFlush() as we specify
    // double buffering in main()
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033: // ASCII code for Esc
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
    //glutInitDisplayMode(GLUT_RGBA);
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

/** File name: q2matrix.cpp
 *
 * Sierpinski gasket with vertex arrays
 *  
 * In this program, we demonstrate how to create 2 buffer objects and how to
 * define the binding point and the data content for each buffer object.
 *
 * Du Huynh
 * August 2015
 */

#include "Angel.h"

using namespace std;

GLuint x2x_x2y_y2x_y2y;

//const int NumPoints = 40000000;
const int NumPoints = 400000;

//----------------------------------------------------------------------------

vec2 points[NumPoints];
vec3 colors[NumPoints];

void init(void)
{
    // Specify the vertices for a triangle
    vec2 vertices[3] = {
        vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
    };

    points[0] = vec2(0.25, 0.50);
    colors[0] = vec3(0.5, 1.0, 1.0);

    vec3 vertex_colors[3] = {
        vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)
    };

    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 3;   // pick a vertex at random

        points[i] = (points[i - 1] + vertices[j]) / 2.0;
        colors[i] = colors[i - 1]*0.5 + vertex_colors[j]*0.5;
    }

    GLuint program = InitShader("q2matrix-v.glsl", "fpassthru.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer[2];
    glGenBuffers(2, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));


    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    x2x_x2y_y2x_y2y = glGetUniformLocation(program, "x2x_x2y_y2x_y2y");

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    
    mat2 x2x_x2y_y2x_y2y_mat = mat2(cos(angle), -sin(angle),
                                    sin(angle),  cos(angle));
    
    glUniformMatrix2fv(x2x_x2y_y2x_y2y, 1, GL_FALSE, x2x_x2y_y2x_y2y_mat);
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

/* Sierpinski gasket with a vertex array and a colour array plus rotation */

#include "Angel.h"

using namespace std;

GLuint mult_ID;

//const int NumPoints = 40000000;
const int NumPoints = 1000000;

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
    colors[0] = vec3(0.0,0.0,0.0);

    for (int i = 1; i < NumPoints; ++i) {
        int j = rand() % 3;   // pick a vertex at random

        points[i] = (points[i - 1] + vertices[j]) / 2.0;
        colors[i][2] = colors[i-1][1];
        colors[i][1] = colors[i-1][0];
        colors[i][0] = 0.25 * j + 0.5 * colors[i-1][2];         
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

    GLuint program = InitShader("q2cMatrixMult-v.glsl", "fpassthru.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)));

    mult_ID = glGetUniformLocation(program, "multipliers");

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);

    mat2 multipliers_mat = mat2(vec2(0.1*cos(angle), -0.1*sin(angle)),
                                 vec2(0.2*sin(angle),  0.2*cos(angle)));
        
    glUniformMatrix2fv(mult_ID, 1, GL_TRUE, multipliers_mat);
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

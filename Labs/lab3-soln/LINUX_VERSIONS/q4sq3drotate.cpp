/* Sierpinski gasket with a vertex array and a colour array plus rotation */

#include "Angel.h"

using namespace std;

const int NumTriangles = 2;
const int NumVertices  = 3 * NumTriangles;

vec3 points[NumVertices] = {
    vec3(-0.5, -0.5, 0.0), vec3(-0.5, 0.5, 0.0), vec3(0.5, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0), vec3(0.5, -0.5, 0.0), vec3(-0.5, 0.5, 0.0)
}; 

vec3 colors[NumVertices] = {
    vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 
    vec3(0.0, 1.0, 1.0), vec3(1.0, 0.0, 1.0), vec3(1.0, 1.0, 0.0), 
};

GLuint transformID; 


int Index = 0;

//----------------------------------------------------------------------------

void init(void)
{
    // Create a vertex array object
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

    GLuint program = InitShader("q3xyzTransform.glsl", "fpassthru.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)));

    transformID = glGetUniformLocation(program, "transform");

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    
    mat3 mTransform = mat3(cos(angle), 0.0, -sin(angle),
                           0.0,        1.0,         0.0,
                           sin(angle), 0.0, cos(angle));
    
    glUniformMatrix3fv(transformID, 1, GL_FALSE, mTransform);

    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
    glutCreateWindow("Simple GLSL example");

    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"

const int NumPoints = 9;

//----------------------------------------------------------------------------

void init(void)
{
    vec2 points[NumPoints] = {
        vec2(-0.1, -0.1), vec2(-0.1, 0.0), vec2(-0.1, 0.1), 
        vec2(0.0, -0.1),  vec2(0.0, 0.0), vec2(0.0, 0.1), 
        vec2(0.1, -0.1),  vec2(0.1, 0.0), vec2(0.1, 0.1)
    };

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);             // clear the window
    glDrawArrays(GL_POINTS, 0, NumPoints);    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033: // ASCII code for the Esc
        exit(EXIT_SUCCESS);
        break;
    }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(256, 256 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutCreateWindow("CITS3003 Lab1 Q2 Points Scene");

    glewInit();  // optional for Linux

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

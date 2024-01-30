/** Program to animate rotating rectangular boxes. We make use of the
 *  RotateX, RotateY, and Scale functions provided in mat.h. We learn
 *  how to use the Frustum function which gives us a clipping volume
 *  of a truncated pyramid shape. This gives us perspective projection.
 *  Notice that the edges of the boxes are not parallel under this
 *  projection.
 *
 *  A reshape callback function is implemented so, when the window is
 *  resized, the rectangular boxes retain their shapes. Compare the reshape
 *  callback function here with that in q3reshapeAndMouse.cpp. 
 *
 *  A timer callback function is implemented so the title of the window
 *  is continuously updated.
 *
 *  Another difference in this program is the glDrawElements function is used
 *  instead of glDrawArrays.
 *
 *  In the sample solution here, we also show how to request and use
 *  two buffer objects. Details are in the 'init' function.
 *
 *  Du Huynh
 *  April 2016
 */

#include "Angel.h"

using namespace std;


// Perspective projection
mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);

// Move the scene backward relative to the camera
mat4 view = Translate(0.0, 0.0, -1.5);

GLint windowHeight=512, windowWidth=512;

int numDisplayCalls = 0;
char lab[] = "Lab5";
char *programName = NULL;


const int NumTriangles = 12;
const int NumVertices  = 8;
const int NumElements  = 3 * NumTriangles;

const float Rad2Deg = 180.0/3.1416;

// Each vertex now appears only once, so we have only 8 rather than 36
vec3 points[NumVertices] = {
    vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
    vec3( -0.5,  0.5, -0.5 ), vec3( -0.5,  0.5,  0.5 ),

    vec3(  0.5, -0.5, -0.5 ), vec3(  0.5, -0.5,  0.5 ),
    vec3(  0.5,  0.5, -0.5 ), vec3(  0.5,  0.5,  0.5 )
};


// The following builds triangles from the 8 vertices above,
// using numbers 0-7 to refer to the element positions in the array
GLuint elements[NumElements] = {
    1, 5, 3,
    7, 3, 5,

    0, 4, 2,
    6, 2, 4,

    4, 6, 5,
    7, 5, 6,

    0, 2, 1,
    3, 1, 2,

    2, 3, 6,
    7, 6, 3,

    0, 1, 4,
    5, 4, 1
};


// We only need to give 8 colors, one for each vertex.
vec3 colors[NumVertices] = {
    vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0),
    vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0),

    vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 1.0),
    vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0),
};


GLuint xyzwMultipliers; 

//----------------------------------------------------------------------------

void init( void )
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "v-xyzw.glsl", "fpassthru.glsl" );
    glUseProgram( program );

    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer[2];
    glGenBuffers( 2, buffer );

    // First, we set up buffer[0] for the vertex position coordinates
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    // Initialize the vertex position attribute for the vertex shader    
    // while buffer[0] is still bound.
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // Next, we set up buffer[1] for the vertex color coordinates
    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // we need a buffer object for the index data
    GLuint indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    // an alternative is to make the 'buffer' variable above an array of 3
    // elements.

    xyzwMultipliers = glGetUniformLocation(program, "xyzwMultipliers");

    // We need to enable the depth test to discard fragments that
    // are behind previously drawn fragments for the same pixel.
    glEnable( GL_DEPTH_TEST );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void drawCube(mat4 model)
{
    glUniformMatrix4fv( xyzwMultipliers, 1, GL_TRUE, projection * view * model );

    // glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_INT, NULL);
}

//----------------------------------------------------------------------------

void display( void )
{
    numDisplayCalls++;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
 
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    float angleDegrees = angle * Rad2Deg;

    // the next 4 drawCube function calls draw a rotated cube at the
    // bottom-left, top-left, bottom-right, and top-right portions of
    // the window respectively.
    drawCube( Translate(-0.5, -0.5, 0) * Scale(0.3) * 
              RotateX(angleDegrees) * RotateY(angleDegrees) );

    drawCube( Translate(-0.5, 0.5, 0) * Scale(0.3) * 
              RotateX(angleDegrees) * RotateY(angleDegrees) );

    drawCube( Translate(0.5, -0.5, 0) * Scale(0.3) * 
              RotateX(angleDegrees) * RotateY(angleDegrees) );

    drawCube( Translate(0.5, 0.5, 0) * Scale(0.3) * 
              RotateX(angleDegrees) * RotateY(angleDegrees) );

    // for the next 3 drawCube function calls, we try to insert the
    // Translate(0.6, 0.6, 0.6) matrix at different location of the
    // transformation 
    drawCube( Translate(0.5, 0.5, 0) * Scale(0.3) *
              RotateX(angleDegrees) * RotateY(angleDegrees) * Translate(0.6, 0.6, 0.6) );

    drawCube( Translate(-0.5, 0.5, 0) * Scale(0.3) *
              RotateX(angleDegrees) * Translate(0.6, 0.6, 0.6) * RotateY(angleDegrees) );

    drawCube( Translate(0.5, -0.5, 0) * Scale(0.3) * 
              Translate(0.6, 0.6, 0.6) * RotateX(angleDegrees) * RotateY(angleDegrees) );
    
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

void idle( void )
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mouse ( int xPixel, int yPixel )
{
    GLfloat x = (GLfloat)xPixel/(GLfloat)windowWidth - 0.5;
    GLfloat y = 0.5 - (GLfloat)yPixel/(GLfloat)windowHeight;

    view = Translate(x, y, -1.5);
}

//----------------------------------------------------------------------------

void reshape( int width, int height )
{
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);

    if (width > height)
        projection = Frustum(-0.2*(float)width/(float)height,
                             0.2*(float)width/(float)height,
                             -0.2, 0.2, 0.2, 2.0);
    else
        projection = Frustum(-0.2, 0.2,
                             -0.2*(float)height/(float)width,
                             0.2*(float)height/(float)width,
                             0.2, 2.0);
}

//----------------------------------------------------------------------------

void timer(int unused)
{
    char title[256];
    sprintf(title, "%s %s: %d Frames Per Second @ %d x %d",
            lab, programName, numDisplayCalls, windowWidth, windowHeight );
 
    glutSetWindowTitle(title);
    
    numDisplayCalls = 0;
    glutTimerFunc(1000, timer, 1);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    // Get the program name, excluding the directory, for the window title
    programName = argv[0];
    for (char *cpointer = argv[0]; *cpointer != 0; cpointer++)
        if (*cpointer == '/' || *cpointer == '\\')
            programName = cpointer+1;
         
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( windowHeight, windowWidth );

    glutCreateWindow( "Wait a sec.." );

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );
    glutMotionFunc( mouse);
    glutReshapeFunc( reshape );
    glutTimerFunc(1000, timer, 1);

    glutMainLoop();
    return 0;
}

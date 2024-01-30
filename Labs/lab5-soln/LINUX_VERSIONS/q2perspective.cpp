/** Program to animate rotating rectangular boxes. We make use of the
 *  RotateX, RotateY, and Scale functions provided in mat.h. We learn
 *  how to use the Frustum function which gives us a clipping volume
 *  of a truncated pyramid shape. This gives us perspective projection.
 *  Notice that the edges of the boxes are not parallel under this
 *  projection.
 *
 *  As no reshape callback function is implemented, when the window is
 *  resized, the rectangular boxes change their shapes.
 *
 *  In the sample solution here, we also show how to request and use
 *  two buffer objects. Details are in the 'init' function.
 *
 *  Du Huynh
 *  April 2016
 */

#include "Angel.h"

/* Perspective projection */
mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);

/* Note that the camera is at the origin, pointing toward the negative-z
 * axis. We need to set up the scene so that it falls inside the clipping
 * volume of the camera. This means that we need to move the scene backward
 * relative to the camera. The 'view' matrix computed below should be the
 * last transformation to apply to the vertex coordinates before the
 * perspective project.
 */
mat4 view = Translate(0.0, 0.0, -1.5);

const int NumTriangles = 12;
const int NumVertices  = 3 * NumTriangles;

const float Rad2Deg = 180.0/3.1416;

vec3 points[NumVertices] = {
    vec3( -0.5, -0.5, 0.5 ), vec3(  0.5, -0.5, 0.5 ), vec3( -0.5, 0.5, 0.5 ),
    vec3(  0.5,  0.5, 0.5 ), vec3( -0.5,  0.5, 0.5 ), vec3( 0.5, -0.5, 0.5 ),

    vec3( -0.5, -0.5, -0.5 ), vec3(  0.5, -0.5, -0.5 ), vec3( -0.5, 0.5, -0.5 ),
    vec3(  0.5,  0.5, -0.5 ), vec3( -0.5,  0.5, -0.5 ), vec3( 0.5, -0.5, -0.5 ),
   
    vec3( 0.5, -0.5, -0.5 ), vec3(  0.5, 0.5, -0.5 ), vec3( 0.5, -0.5, 0.5 ),
    vec3( 0.5, 0.5,  0.5 ), vec3( 0.5, -0.5,  0.5 ), vec3( 0.5, 0.5, -0.5 ),

    vec3( -0.5, -0.5, -0.5 ), vec3( -0.5,  0.5, -0.5 ), vec3( -0.5, -0.5, 0.5 ),
    vec3( -0.5,  0.5,  0.5 ), vec3( -0.5, -0.5,  0.5 ), vec3( -0.5,  0.5, -0.5 ),

    vec3( -0.5, 0.5, -0.5 ), vec3( -0.5, 0.5,  0.5 ), vec3(  0.5, 0.5, -0.5 ),
    vec3(  0.5, 0.5,  0.5 ), vec3(  0.5, 0.5, -0.5 ), vec3( -0.5, 0.5, 0.5 ),

    vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ), vec3(  0.5, -0.5, -0.5 ),
    vec3(  0.5, -0.5,  0.5 ), vec3(  0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
}; 

vec3 colors[NumVertices] = {
    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 

    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 

    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 

    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 

    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 

    vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), 
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
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

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
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void display( void )
{
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

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );

    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Lab5 q2" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}

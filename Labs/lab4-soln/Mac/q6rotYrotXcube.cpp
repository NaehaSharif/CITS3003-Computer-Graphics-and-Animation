#include "Angel.h"

using namespace std;

const int NumTriangles = 12;
const int NumVertices  = 3 * NumTriangles;

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

GLuint xyzMultipliers; 

//----------------------------------------------------------------------------

void init( void )
{
    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // First, we create an empty buffer of the size we need by passing
    //   a NULL pointer for the data values
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
                  NULL, GL_STATIC_DRAW );

    // Next, we load the real data in parts.
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "v-xyz.glsl", "fpassthru.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    // Likewise, initialize the vertex color attribute.
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)) );

    xyzMultipliers = glGetUniformLocation(program, "xyzMultipliers");

    // We need to enable the depth test to discard fragments that
    // are behind previously drawn fragments for the same pixel.
    glEnable( GL_DEPTH_TEST );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
 
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    
    mat3 rotateYmultipliers =  mat3( vec3(cos(angle),  0.0, sin(angle)),
                                     vec3(0.0,         1.0,         0.0),
                                     vec3(-sin(angle), 0.0,  cos(angle)) );

    mat3 rotateXmultipliers =  mat3( vec3(1.0,        0.0,         0.0),
                                     vec3(0.0, cos(angle), -sin(angle)),
                                     vec3(0.0, sin(angle),  cos(angle)) );

    mat3 combinedMultipliers = rotateYmultipliers * rotateXmultipliers;
     
    glUniformMatrix3fv( xyzMultipliers, 1, GL_TRUE, combinedMultipliers ); 
 /*  the third parameter in glUniformMatrix3fv() specifies whether to transpose the matrix as the values are loaded into the uniform variable. 
If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order. If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order. 
We set it GL_TRUE to ensure correct matrix-vector mulitplication in the shader i.e., combinedMultipliers[0] corresponds to the first row of the matrix.
If we were to set it to GL_FALSE then the matrix-vector multipliation in the shader would equate to combinedMultipliers^T * vPosition instead of combinedMultipliers* vPosition.
Also, our C++ matrices are arrays of rows, while GLSL has arrays of columns.*/
    
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
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

    glutCreateWindow( "Lab4 q6 rotY*rotX cube" );

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}

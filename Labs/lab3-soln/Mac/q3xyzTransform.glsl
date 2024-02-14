#version 150

in  vec3 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat3 transform;

void main()
{
    gl_Position = vec4(transform * vPosition, 1.0);
    color = vec4( vColor, 1.0 );
}

/* Note: By declaring vPosition as vec3 in the vertex shader, we
 * effectively pad a 0 value to the z-component of vPosition. If vPosition
 * is (2.0, 3.0) in the q3yzRotate.cpp, then the vPosition here would be
 * (2.0, 3.0, 0.0). The code above works the same as the following:

 * The version below works for vec2 only.

version 150

in  vec2 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat3 transform;

void main()
{
    vec3 pos = vec3(vPosition, 0.0);
    gl_Position = vec4(transform * pos, 1.0);
    color = vec4( vColor, 1.0 );
}


*/


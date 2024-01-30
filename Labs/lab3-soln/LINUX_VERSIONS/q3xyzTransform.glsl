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

/* Note: this vertex shader is used by q3yzRotate.cpp. In there, vPosition is
 * passed in as a vec2. So the in variable 'vPosition' declared above should
 * be of type vec2. By declaring vPosition as vec3 in the vertex shader, we
 * effectively pad a 0 value to the z-component of vPosition. If vPosition
 * is (2.0, 3.0) in the q3yzRotate.cpp, then the vPosition here would be
 * (2.0, 3.0, 0.0). The code above works the same as the following:
 *
 * To be safe, the code below should be used. However, since this glsl file
 * is used by q3xyzTransform.cpp, q3xyzTransform2.cpp, q4cube.cpp, q4sq3drotate.cpp,
 * and q4threeD.cpp where vPosition is passed in as vec2 in some .cpp files
 * but as vec3 in some others, the version above would work for both cases.
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


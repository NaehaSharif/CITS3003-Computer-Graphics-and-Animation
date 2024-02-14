#version 150

in  vec3 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat4 xyzwMultipliers;

void main()
{
    gl_Position = xyzwMultipliers * vec4(vPosition, 1.0);

    color = vec4( vColor, 1.0 );
}

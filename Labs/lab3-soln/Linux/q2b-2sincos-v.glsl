#version 150

in  vec2 vPosition;

uniform float sinAngle, cosAngle;

void main()
{
    gl_Position = vec4(vPosition.x*cosAngle - vPosition.y*sinAngle,
                       vPosition.x*sinAngle + vPosition.y*cosAngle,
                       0.0,
                       1.0);
}

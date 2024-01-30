attribute vec2 vPosition;
attribute vec3 vColor;
varying vec4 color;

uniform mat2 multipliers;

void main()
{
    gl_Position = vec4(multipliers * vPosition, 0.0, 1.0);
    color = vec4( vColor, 1.0 );
}

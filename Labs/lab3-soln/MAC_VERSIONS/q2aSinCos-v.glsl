attribute vec2 vPosition;
attribute vec3 vColor;
varying vec4 color;

uniform float sinAngle, cosAngle;

void main()
{
    gl_Position = vec4(vPosition.x*cosAngle - vPosition.y*sinAngle,
                       vPosition.x*sinAngle + vPosition.y*cosAngle,
                       0.0,
                       1.0);
    color = vec4( vColor, 1.0 );
}

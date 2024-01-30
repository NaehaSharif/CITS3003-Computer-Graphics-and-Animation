attribute vec2 vPosition;
attribute vec3 vColor;
varying vec4 color;

uniform mat2 x2x_x2y_y2x_y2y;

void main()
{
    gl_Position.xy = x2x_x2y_y2x_y2y * vPosition;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    color = vec4( vColor, 1.0 );
}

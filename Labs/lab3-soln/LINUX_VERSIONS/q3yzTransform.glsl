#version 150

in  vec2 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat2 x2x_x2y_y2x_y2y;

void main()
{
    gl_Position.yz = x2x_x2y_y2x_y2y * vPosition.xy;
    gl_Position.xw = vec2(vPosition.x, 1.0);

    color = vec4( vColor, 1.0 );
}

#version 150

in vec2 vPosition;

void main()
{
    gl_Position.xy = vPosition;
    gl_Position.zw = vec2(0.0, 1.0);
}

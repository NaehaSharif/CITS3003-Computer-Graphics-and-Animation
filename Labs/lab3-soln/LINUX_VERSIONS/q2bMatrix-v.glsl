#version 150

in  vec2 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat2 multipliers;

void main()
{
    gl_Position =
        vec4(vPosition.x*multipliers[0][0] + vPosition.y*multipliers[1][0], 
             vPosition.x*multipliers[0][1] + vPosition.y*multipliers[1][1],
             0.0,
             1.0);

    color = vec4( vColor, 1.0 );
}

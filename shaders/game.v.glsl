#version 410
// ins (inputs)
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;

// outs
out vec4 color;

void main()
{
    vec4 positionH = vec4(vertexPosition,0,1);
    gl_Position =  positionH;
    color = vertexColor;
}

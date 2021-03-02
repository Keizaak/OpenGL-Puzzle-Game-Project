#version 410

layout(location = 1) in vec4 color ;
out vec4 fragColor ; 

void main()
{
    fragColor=color;
}

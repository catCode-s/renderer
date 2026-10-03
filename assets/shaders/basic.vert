// source code for vertex shader
#version 330 core

uniform mat4 mvp;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

out vec3 vertexColor;
out vec2 fragUV;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    vertexColor = color;
    fragUV = uv;
}
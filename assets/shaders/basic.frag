// source code for fragment shader
#version 330 core

uniform float time;
uniform sampler2D texture1;

in vec3 vertexColor;
in vec2 fragUV;

out vec4 fragmentColor;

void main()
{
    float red = sin(time) * 0.5 + 0.5;
    vec4 texColor = texture(texture1, fragUV);

    fragmentColor =
        vec4(red, vertexColor.g, vertexColor.b, 1.0) * texColor;
}
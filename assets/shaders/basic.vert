// source code for vertex shader
#version 330 core

uniform mat4 mvp;
uniform mat4 model;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

out vec2 fragUV;
out vec3 fragPos;
out vec3 fragNormal;

void main() {
    gl_Position = mvp * vec4(position, 1.0);

    fragUV = uv;

    // convert to world space
    fragPos = vec3(model * vec4(position, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    fragNormal = normalMatrix * normal;
}

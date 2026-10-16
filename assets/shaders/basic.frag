// source code for fragment shader
#version 330 core

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;

out vec4 fragmentColor;

void main() {
    float ambient = 0.2;
    float specularIntensity = 0.5;
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);

    float specular = specularIntensity * spec;

    //float red = sin(time) * 0.5 + 0.5;
    vec4 texColor = texture(texture1, fragUV);

    //vec4 baseColor = vec4(0.0, vertexColor.g, vertexColor.b, 1.0) * texColor;
     vec4 baseColor = vec4(0.0, 0.8, 0.2, 1.0) * texColor;

    vec3 result = baseColor.rgb * vec3(ambient + diff) + vec3(specular);

    fragmentColor = vec4(result, baseColor.a);
}
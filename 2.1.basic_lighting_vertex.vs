#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec3 VextexColor;

void main() {
    vec3 FragPos  = vec3(model * vec4(aPos, 1.0));

    gl_Position   = projection * view * vec4(FragPos, 1.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient  = ambientStrength * lightColor;

    // diffuse
    vec3 norm     = normalize(aNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    //float diff    = max(dot(norm, lightDir), 0.0);
    float diff    = clamp(dot(norm, lightDir), 0, 1);
    vec3 diffuse  = diff * lightColor;

    VextexColor   = (ambient + diffuse) * objectColor;
}
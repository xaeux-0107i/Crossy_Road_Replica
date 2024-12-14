#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

uniform vec3 lightPos;   // ���� ��ġ
uniform vec3 viewPos;    // ī�޶� ��ġ
uniform vec3 lightColor; // ���� ����
uniform vec3 objectColor; // ��ü ����
uniform int isBlending; // ���� ����
uniform float light_strength;

void main() {
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor * light_strength;
    fragColor = vec4(result, 1.0);
    if (isBlending == 1) fragColor = vec4(result, 0.5);
    if (isBlending == 2) fragColor = vec4(result, 0.7);
}

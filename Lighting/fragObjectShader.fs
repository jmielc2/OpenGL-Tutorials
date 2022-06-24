#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;
out vec4 FragColor;

void main()
{
    const float ambientStrength = 0.15f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}
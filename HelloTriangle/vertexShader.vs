#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 pointColor;

uniform float scale;

void main()
{
    pointColor = aColor;
    gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.x * scale, 1.0);
}
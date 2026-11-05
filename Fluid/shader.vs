#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aSpd;

out vec2 vSpd;

uniform vec2 marimeEcran;
uniform float raza;

void main()
{
    vec2 repozitionare = aPos / marimeEcran * 2.0 - 1.0; 
    gl_Position = vec4(repozitionare.x, -repozitionare.y, 0.0, 1.0);
    gl_PointSize = raza * 2.0;

    vSpd = aSpd;
}
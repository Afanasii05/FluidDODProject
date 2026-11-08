#version 330 core

out vec4 FragColor;
in vec2 vSpd;

uniform float spd_mult;
uniform vec2 norm_spd;

void main() {
    vec2 c = gl_PointCoord * 2.0 - 1.0;
    if (dot(c, c) > 1.0) { discard; } // nu desenez pixelii din exteriorul razei

    float angle = atan(vSpd.y, vSpd.x);

    float r = sin(angle) * 0.5 + 0.5;
    float g = sin(angle + 2.09439) * 0.5 + 0.5; // 2.09439 - 120 grade
    float b = sin(angle + 4.18879) * 0.5 + 0.5; // 4,18879 - 240 grade


    FragColor = vec4(r, g, b, 1.0);
}
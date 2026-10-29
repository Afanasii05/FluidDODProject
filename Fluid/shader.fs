#version 330 core
out vec4 FragColor;

void main() {
    vec2 c = gl_PointCoord * 2.0 - 1.0;  
    if (dot(c, c) > 1.0)                  
        discard;                          // nu desenez pixelii din exteriorul razei
    FragColor = vec4(0.3, 0.6, 0.6, 1.0);   
}
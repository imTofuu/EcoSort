#version 410 core

in vec3 f_normal;
in vec2 f_uv;

uniform sampler2D texture1;

out vec4 FragColor;

void main() {
    FragColor = texture(texture1, f_uv);
    
    
}

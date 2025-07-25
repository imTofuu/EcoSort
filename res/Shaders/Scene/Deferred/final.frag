#version 410 core

in vec2 f_uv;

uniform sampler2D u_screen;

out vec4 fragColor;

void main() {
    fragColor = texture(u_screen, f_uv);
}
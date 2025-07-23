#version 410 core

in vec2 f_uv;

uniform sampler2D u_screen;

out vec4 fragColor;

void main() {
    fragColor = vec4(texture(u_screen, f_uv).rgb, 1);
}
#version 410 core

in vec2 f_uv;

uniform sampler2D screen;

out vec4 fragColor;

void main() {
    fragColor = vec4(texture(screen, f_uv).rgb, 1);
}
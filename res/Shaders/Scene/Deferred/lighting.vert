#version 410 core

layout (location = 0) in vec3 a_position;

out vec2 f_uv;

void main() {
    gl_Position = vec4(a_position, 1.0);

    f_uv = (a_position.xy * 0.5) + 0.5;
}
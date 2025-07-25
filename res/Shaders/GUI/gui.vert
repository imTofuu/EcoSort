#version 410 core

layout (location = 0) in vec3 a_position;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 f_uv;

void main() {
    gl_Position = u_projection * u_model * vec4(a_position, 1.0);

    f_uv = vec2(a_position.x, 1 - a_position.y);
}
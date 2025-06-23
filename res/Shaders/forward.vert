#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 f_normal;
out vec2 f_uv;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
    
    f_normal = a_normal;
    f_uv = a_uv;
}

#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 f_normal;
out vec2 f_uv;
out vec3 f_fragPos;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
    
    f_normal = mat3(transpose(inverse(u_model))) * a_normal;
    f_uv = a_uv;
    f_fragPos = vec3(u_model * vec4(a_position, 1.0));
}

#version 410 core

in vec3 f_normal;
in vec2 f_uv;
in vec3 f_fragPos;

uniform sampler2D u_primaryTexture;

layout (location = 0) out vec4 o_positions;
layout (location = 1) out vec4 o_normals;
layout (location = 2) out vec4 o_albedo;

void main() {

    o_positions = vec4(f_fragPos, 1);
    o_normals = vec4(normalize(f_normal), 1);
    o_albedo = texture(u_primaryTexture, f_uv);
    
}
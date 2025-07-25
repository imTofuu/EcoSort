#version 410 core

in vec3 v_normal;
in vec2 v_uv;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 color1 = texture(texture1, v_uv);
    //vec4 color2 = texture(texture2, v_uv);
    
    FragColor = color1;//mix(color1, color2, 0.5);
}
#version 410 core

in vec2 f_uv;

uniform sampler2D u_image;
uniform vec4 u_colour;

out vec4 fragColour;

void main() {
    fragColour = texture(u_image, f_uv) * u_colour;
}
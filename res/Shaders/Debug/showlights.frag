#version 410 core

uniform vec3 u_lightColour;

out vec4 fragColour;

void main() {
    fragColour = vec4(u_lightColour, 1.0);
}
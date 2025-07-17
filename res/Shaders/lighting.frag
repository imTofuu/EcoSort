#version 410 core

#define AMBIENT_STRENGTH 0.5
#define AMBIENT_COLOUR normalize(vec3(1, 0, 0))

in vec2 f_uv;

uniform sampler2D gPositions;
uniform sampler2D gNormals;
uniform sampler2D gAlbedos;

uniform vec3 u_lightPosition;

out vec4 fragColour;

void main() {
    
    vec3 position = texture(gPositions, f_uv).xyz;
    vec3 normal = texture(gNormals, f_uv).xyz;
    vec3 albedo = texture(gAlbedos, f_uv).xyz;

    vec3 ambient = AMBIENT_COLOUR * albedo * AMBIENT_STRENGTH;
    
    vec3 lightDir = normalize(u_lightPosition - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedo;
    
    fragColour = vec4(diffuse, 1);
}
#version 410 core

#define AMBIENT_STRENGTH 0.1
#define AMBIENT_COLOUR normalize(vec3(1, 1, 1))

in vec2 f_uv;

uniform sampler2D u_gPositions;
uniform sampler2D u_gNormals;
uniform sampler2D u_gAlbedos;

uniform vec3 u_lightPosition;

out vec4 fragColour;

void main() {
    
    vec3 position = texture(u_gPositions, f_uv).xyz;
    vec3 normal = texture(u_gNormals, f_uv).xyz;
    vec3 albedo = texture(u_gAlbedos, f_uv).xyz;

    vec3 ambient = AMBIENT_COLOUR * AMBIENT_STRENGTH;
    
    vec3 lightDir = normalize(u_lightPosition - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedo;
    
    fragColour = vec4(ambient + diffuse, 1);
}
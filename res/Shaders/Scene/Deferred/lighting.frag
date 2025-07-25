#version 410 core

#define AMBIENT_STRENGTH 0.2
#define AMBIENT_COLOUR normalize(vec3(1, 1, 1))

in vec2 f_uv;

uniform sampler2D u_gPositions;
uniform sampler2D u_gNormals;
uniform sampler2D u_gAlbedos;

struct Light {
    int lightType; // 0: point, 1: spot, 2: directional
    vec3 position;
    vec4 direction; // quaternion
    vec3 colour;
};

uniform Light u_light;

out vec4 fragColour;

void main() {
    
    vec3 position = texture(u_gPositions, f_uv).xyz;
    vec3 normal = texture(u_gNormals, f_uv).xyz;
    vec3 albedo = texture(u_gAlbedos, f_uv).xyz;

    vec3 ambient = AMBIENT_COLOUR * albedo * AMBIENT_STRENGTH;
    
    vec3 result = ambient;
    
    switch (u_light.lightType) {
        case 0:
            vec3 lightDir = normalize(u_light.position - position);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * albedo * u_light.colour;
            
            result += diffuse;
            break;
        default:
            result = vec3(1, 0, 0);
            break;
    }
    
    fragColour = vec4(result, 1);
}
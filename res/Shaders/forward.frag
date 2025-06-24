#version 410 core

#define AMBIENT_STRENGTH 0.5
#define AMBIENT_COLOR normalize(vec3(0.1, 0.1, 0.1))

in vec3 f_normal;
in vec2 f_uv;
in vec3 f_fragPos;

uniform sampler2D texture1;

out vec4 FragColor;

vec3 lightPosition = vec3(0.0, 0.0, 0.0);
vec3 lightColour = vec3(1.0, 0.0, 0.0);

void main() {
    
    vec3 ambient = AMBIENT_COLOR * AMBIENT_STRENGTH;
    
    vec3 lightDir = normalize(lightPosition - f_fragPos);
    float diff = max(dot(f_normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    vec4 result = vec4(ambient + diffuse, 1.0f) * texture(texture1, f_uv);
    
    FragColor = result;
    
}

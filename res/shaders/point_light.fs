#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec2 pos;
    vec3 color;

    float linear;
    float quadratic;
};

uniform Light light;
uniform vec2 viewPos;

void main() {
    // Retrieve data from g-buffer
    vec2 fragPos = texture(gPosition, texCoords).rg;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
    diffuse *= attenuation;

    FragColor = vec4(diffuse, 1);
}

#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;
uniform mat4 proj;
uniform mat4 view;
uniform vec2 windowSize;

struct Light {
    vec2 pos;
    vec3 color;

    float radius;
    float intensity;
};

uniform Light light;
uniform vec2 viewPos;

void main() {
    // Retrieve data from g-buffer
    vec2 fragPos = texture(gPosition, texCoords).rg;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;
    vec4 spacePos = proj * (view * vec4(light.pos, 0, 1));
    vec3 ndcSpacePos = spacePos.xyz / spacePos.w;
    vec2 windowSpacePos = ((ndcSpacePos.xy + 1) / 2) * windowSize;

    float distance = distance(fragPos, light.pos);
    float attenuation = max(0.0, 1.0 - distance / light.radius);
    diffuse *= (attenuation * attenuation * light.intensity * light.color);

    FragColor = vec4(diffuse, 1);
}

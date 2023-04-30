#version 410 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;

uniform float brightness;

void main() {
    // Retrieve data from g-buffer
    vec2 fragPos = texture(gPosition, texCoords).rg;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;

    vec3 nightColor = vec3(0.1, 0.1, 0.25);
    vec3 middleColor = vec3(0.9, 0.5, 0.2);
    vec3 dayColor = vec3(1);
    diffuse *= mix(mix(nightColor, middleColor, brightness), mix(middleColor, dayColor, brightness), brightness);

    FragColor = vec4(diffuse, 1);
}

#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;

uniform float brightness;

void main() {
    // Retrieve data from g-buffer
    vec2 fragPos = texture(gPosition, texCoords).rg;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;

    diffuse *= mix(vec3(0.1, 0.1, 0.25), vec3(1), brightness);

    FragColor = vec4(diffuse, 1);
}

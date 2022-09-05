#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

layout (location = 0) out vec4 Color;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out float TexIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1);
    gl_Position = projection * view * worldPos;

    Color = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    TexIndex = aTexIndex;
}

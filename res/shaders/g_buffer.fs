#version 460 core

//out vec4 FragColor;
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedoSpec;

in vec4 Color;
in vec2 TexCoord;
in float TexIndex;

in vec4 fragPos;

// Texture samplers
uniform sampler2D textures[16];

void main()
{
    // It turned out that it's undefined behavior in glsl.
    // Yes, in most cases it works, but, as we already know, not always
    //int index = int(TexIndex);
    //FragColor = texture(textures[index], TexCoord) * Color;
    // So we have to do this scary thing 👍
    vec4 fragColor;

    int index = int(TexIndex);
    switch (index) {
        case 0:
            fragColor = texture(textures[0], TexCoord) * Color;
            break;
        case 1:
            fragColor = texture(textures[1], TexCoord) * Color;
            break;
        case 2:
            fragColor = texture(textures[2], TexCoord) * Color;
            break;
        case 3:
            fragColor = texture(textures[3], TexCoord) * Color;
            break;
        case 4:
            fragColor = texture(textures[4], TexCoord) * Color;
            break;
        case 5:
            fragColor = texture(textures[5], TexCoord) * Color;
            break;
        case 6:
            fragColor = texture(textures[6], TexCoord) * Color;
            break;
        case 7:
            fragColor = texture(textures[7], TexCoord) * Color;
            break;
        case 8:
            fragColor = texture(textures[8], TexCoord) * Color;
            break;
        case 9:
            fragColor = texture(textures[9], TexCoord) * Color;
            break;
        case 10:
            fragColor = texture(textures[10], TexCoord) * Color;
            break;
        case 11:
            fragColor = texture(textures[11], TexCoord) * Color;
            break;
        case 12:
            fragColor = texture(textures[12], TexCoord) * Color;
            break;
        case 13:
            fragColor = texture(textures[13], TexCoord) * Color;
            break;
        case 14:
            fragColor = texture(textures[14], TexCoord) * Color;
            break;
        case 15:
            fragColor = texture(textures[15], TexCoord) * Color;
            break;
    }

    gPosition = fragPos;
    gAlbedoSpec = fragColor;
}

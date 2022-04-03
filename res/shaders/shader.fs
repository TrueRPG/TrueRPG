#version 460 core

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
in float TexIndex;

// Texture samplers
uniform sampler2D textures[16];

uniform vec2 resolution;

struct LightSource
{
	vec3 color;
	vec2 pos;
	float intensity;
	float radius;
};

const int MAX_LIGHT_SOURCES = 101;

uniform LightSource globalLight;
uniform LightSource lightSources[MAX_LIGHT_SOURCES];

vec3 genLightSource(LightSource light, vec2 fragCoord)
{
    float d = distance(fragCoord, light.pos);
    float a = max(0.0, 1.0 - d / light.radius);
    return a * a * light.intensity * light.color;
}

void main()
{
	vec2 lightPos = resolution.xy / 2;
	vec3 clr = genLightSource(lightSources[0], gl_FragCoord.xy);

	for (int i = 1; i < MAX_LIGHT_SOURCES; ++i)
	{
	    if (lightSources[i].intensity <= 0) continue;
		clr += genLightSource(lightSources[i], gl_FragCoord.xy);
	}

    // It turned out that it's undefined behavior in glsl.
    // Yes, in most cases it works, but, as we already know, not always
    //int index = int(TexIndex);
    //FragColor = texture(textures[index], TexCoord) * Color;
    // So we have to do this scary thing 👍
    int index = int(TexIndex);
    switch (index) {
        case 0:
            FragColor = texture(textures[0], TexCoord) * Color * vec4(clr.rgb, 1.0);
            break;
        case 1:
            FragColor = texture(textures[1], TexCoord) * Color * vec4(clr.rgb, 1.0);
            break;
        case 2:
            FragColor = texture(textures[2], TexCoord) * Color;
            break;
        case 3:
            FragColor = texture(textures[3], TexCoord) * Color;
            break;
        case 4:
            FragColor = texture(textures[4], TexCoord) * Color;
            break;
        case 5:
            FragColor = texture(textures[5], TexCoord) * Color;
            break;
        case 6:
            FragColor = texture(textures[6], TexCoord) * Color;
            break;
        case 7:
            FragColor = texture(textures[7], TexCoord) * Color;
            break;
        case 8:
            FragColor = texture(textures[8], TexCoord) * Color;
            break;
        case 9:
            FragColor = texture(textures[9], TexCoord) * Color;
            break;
        case 10:
            FragColor = texture(textures[10], TexCoord) * Color;
            break;
        case 11:
            FragColor = texture(textures[11], TexCoord) * Color;
            break;
        case 12:
            FragColor = texture(textures[12], TexCoord) * Color;
            break;
        case 13:
            FragColor = texture(textures[13], TexCoord) * Color;
            break;
        case 14:
            FragColor = texture(textures[14], TexCoord) * Color;
            break;
        case 15:
            FragColor = texture(textures[15], TexCoord) * Color;
            break;
    }	
}

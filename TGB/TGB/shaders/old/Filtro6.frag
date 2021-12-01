//Filtro 6 - Sépia

#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord);// * vec4(ourColor, 1.0);
    float media_r = (color.r * 0.393 + color.g * 0.769 + color.b * 0.189);
    float media_g = (color.r * 0.349 + color.g * 0.686 + color.b * 0.168);
    float media_b = (color.r * 0.272 + color.g * 0.534 + color.b * 0.131);
    
    if (media_r > 1.0)
    {
        color.r = 1.0;
    }
    else
    {
        color.r = media_r;
    }

    if (media_g > 1.0)
    {
        color.g = 1.0;
    }
    else
    {
        color.g = media_g;
    }

    if (media_b > 1.0)
    {
        color.b = 1.0;
    }
    else
    {
        color.b = media_b;
    }

    color = vec4(color.r, color.g, color.b, 1.0);
}
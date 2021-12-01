//Filtro 2 - Negativo

#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord);// * vec4(ourColor, 1.0);
    color = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
}
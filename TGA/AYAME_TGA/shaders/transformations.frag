#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

// Anima��o

void main()
{
    color = texture(ourTexture1, TexCoord);// * vec4(ourColor, 1.0);
}
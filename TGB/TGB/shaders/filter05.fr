//Filtro 5 - Negativo + colorização

#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord);// * vec4(ourColor, 1.0);
	color = vec4(1.0 - color.r, 0.5 - color.g, 0.5 - color.b, 1.0);

}
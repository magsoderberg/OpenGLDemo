#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ourTexture;

void main()
{
    //making a comment here
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = texture(ourTexture, TexCoords);
}
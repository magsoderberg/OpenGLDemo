#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D ourTexture;


//struct Material
//{
//    sampler2D diffuse;
//    vec3 specular;
//    float shininess;
//};
//
//uniform Material material;

void main()
{
    //making a comment here
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);

   /* tex_x_shift += 0.01;

    if (tex_x_shift > 1.0) {
        tex_x_shift = 0.0;
    }*/
    //if (TexCoord.x + tex_x_shift > 1.0) {
        FragColor = texture(ourTexture, vec2(TexCoords.x, TexCoords.y));
    //}
    //else {
        //FragColor = texture(material.diffuse, vec2(TexCoord.x + tex_x_shift, TexCoord.y));
    //}
}
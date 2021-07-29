#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    

    //if (shift_texture == 1) {
    //    //TexCoords.x += tex_x_shift;
    //    //TexCoords.y += tex_x_shift;

    //    if (TexCoords.x >= 1.0) {
    //        TexCoords.x = 0.0;
    //    }
    //    if (TexCoords.y >= 1.0) {
    //        TexCoords.y = 0.0;
    //    }
    //}


    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
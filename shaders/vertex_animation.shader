#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aPos2;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aNormal2;
layout(location = 4) in vec2 aTexCoords;
layout(location = 5) in vec2 aTexCoords2;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 nextVertex;
uniform vec3 nextNormal;

uniform float tweenFactor;



void main()
{


    vec3 newPos = mix(aPos, aPos2, tweenFactor);
    //vec3 newNormal = mix(aNormal, aNormal2, tweenFactor);
    
    FragPos = vec3(model * vec4(newPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
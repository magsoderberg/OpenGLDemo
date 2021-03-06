#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic; 
};



struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;



void main()
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //Ambient light
    vec3 ambient =  light.ambient * vec3(texture(material.diffuse, TexCoords));

    //Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    //Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.specular * (spec * material.specular);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
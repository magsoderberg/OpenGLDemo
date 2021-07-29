#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


//This is the properties of the incoming light 
//ambient is the light intensity for ambient light (vec(1.0) would be full intensity, at which point the object would look like its glowing)
//and same for diffuse and specular
//however it is also the color of the light. So usually you want to keep diffuse at a bright white color
//specular is usually kept at vec(1.0)
struct Light
{
    
    vec3 direction;
    vec3 color;

    // spotlight
    vec3 position; 
    float cutoff;
    float outerCutoff;

    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;

    int directional;
    int point;
    int spot;
    int spotColored;

};

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

//uniform sampler2D ourTexture;
uniform int nr_lights;
uniform Material material;
uniform Light lights[10];
uniform vec3 viewPos;

//moving texture
uniform int shift_texture;
uniform float tex_x_shift;
uniform float tex_y_shift;


vec3 CalcPointLight(Light light) {

    //Ambient light
    //vec3 ambient;
    vec3 ambient;
    if (shift_texture == 1) {
        ambient = light.ambient * texture(material.diffuse, vec2(TexCoords.x + tex_x_shift, TexCoords.y + tex_y_shift)).rgb;
    }
    else {
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    }

    //Diffuse lighting
    vec3 norm = normalize(Normal);
    
    //vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse;
    if (shift_texture == 1) {
        diffuse = light.diffuse * diff * texture(material.diffuse, vec2(TexCoords.x + tex_x_shift, TexCoords.y + tex_y_shift)).rgb;
    }
    else {
        diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    }
    

    //Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 CalcSpotLight(Light light) {

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    /*specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoords).rgb;*/

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutoff - light.outerCutoff);
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    //specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;

    vec3 result = ambient + diffuse; // +specular;
    return result;

    
}

vec3 CalcSpotLightColored(Light light) {

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * light.color * diff * texture(material.diffuse, TexCoords).rgb;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoords).rgb;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutoff - light.outerCutoff);
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse +specular;
    return result;

}

void main()
{
    vec3 result;

    for (int i = 0; i < nr_lights; i++) {
        if (lights[i].spot == 1) {
            result += CalcSpotLight(lights[i]);
        }
        else if (lights[i].directional == 1) {
            result += CalcPointLight(lights[i]);
        }
        else if (lights[i].spotColored == 1) {
            result += CalcSpotLightColored(lights[i]);
        }
        else if (lights[i].point == 1) {
            result += CalcPointLight(lights[i]);
        }  
    }
        
    FragColor = vec4(result, 1.0);
}
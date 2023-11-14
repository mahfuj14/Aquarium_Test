#version 330 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;   
};
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in float visibility;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform vec3 FogColor;
uniform bool enableFog;
uniform bool enableDirLight;
uniform bool enableSpotLight;
uniform bool enablePointLight;
uniform samplerCube skybox;
uniform float time;

uniform DirLight dirLight;
//uniform Material material;
uniform SpotLight spotLight;
uniform PointLight pointLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(1.0, 1.0, 1.0);
    if (enablePointLight)
		result *= 9.0 * CalcPointLight(pointLight, norm, FragPos,viewDir);

	if (enableDirLight)
		result += 0.5 * CalcDirLight(dirLight, norm, viewDir);

	if (enableSpotLight)
		result += 2.0 * CalcSpotLight(spotLight, norm, FragPos, viewDir);


    //float ratio = 1.00 / 1.52;
    //vec3 I = normalize(FragPos - viewPos);
    //vec3 R = refract(I, normalize(Normal), ratio);  
    vec3 color = result + vec3(0.1, 0.1, 0.1) * sin(time);
    FragColor = vec4(color, 0.2); // Set transparency to 0.5

	//FragColor = vec4(0.28,0.55,0.44, 0.0) * vec4(result, 1.0);

}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (0.3*ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, normal);
    
    vec3 K_A = vec3(1.0f, 1.0f, 1.0f);
    vec3 K_D = vec3(1.0f, 1.0f, 1.0f);
    vec3 K_S = vec3(0.5f, 0.5f, 0.5f);
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(normal, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(viewDir, R), 0.0), 32.0f) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

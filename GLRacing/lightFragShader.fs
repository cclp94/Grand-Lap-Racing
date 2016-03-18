#version 130

struct Light {
 vec3 direction;

 vec4 ambient;
 vec4 diffuse;
 vec4 specular;
};

struct Material {
 vec4 ambient;
 vec4 diffuse;
 vec4 specular;
 float shininess;
};

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
in vec3 FragPos;  
in vec3 Normal; 
in vec3 out_Color;
out vec4 frag_colour;	//final output color used to render the point
in vec4 FragPosLightSpace;
in vec2 TextCoord;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main () {

	vec3 color = texture(diffuseTexture, TextCoord).rgb;

	vec3 lightDir = -normalize(FragPos - light.direction);


	// Ambient
	 vec4 ambient = light.ambient * material.ambient * vec4(color, 1.0);
	
	 // Diffuse
	 vec3 norm = normalize(Normal);
	 float diff = max(dot(norm, lightDir), 0.0);
	 vec4 diffuse = light.diffuse * (diff * material.diffuse);
	
	 // Specular
	 vec3 viewDir = normalize(viewPos - FragPos);
	 vec3 reflectDir = reflect(-lightDir, norm);
	 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	 vec4 specular = light.specular * (spec * material.specular);

	 float shadow = ShadowCalculation(FragPosLightSpace); 
	
	 vec4 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * vec4(color, 1.0);
	 frag_colour = result;
}
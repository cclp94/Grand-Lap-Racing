#version 330 core

struct Light {
 vec3 direction;

 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

struct Material {
 vec4 ambient;
 vec4 diffuse;
 vec4 specular;
 float shininess;
};


uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
in vec3 FragPos;  
in vec3 Normal; 
layout(location = 0) out vec4 frag_colour;	//final output color used to render the point
in vec4 ShadowCoord;
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
	float bias = 0.005;
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    return shadow;
}


void main () {

	vec3 color = texture(diffuseTexture, TextCoord).rgb;

	vec3 lightDir = normalize(light.direction);



	// Ambient
	 vec3 ambient = light.ambient * color;
	float shadow = ShadowCalculation(ShadowCoord);
	 // Diffuse
	 vec3 norm = normalize(Normal);
	 float diff = max(dot(lightDir, norm), 0.0);
	 vec3 diffuse = (diff) * light.diffuse ;
	 diffuse = max(diffuse, 0.4);
	
	 // Specular
	 vec3 viewDir = normalize(FragPos - viewPos);
	 vec3 reflectDir = reflect(lightDir, norm);
	 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	 vec3 specular = (spec) * light.specular;
	 float bias = 0.005;
	        
    vec3 lighting = (ambient +   ((1.0 - shadow)*diffuse) + specular) * color;
	 frag_colour = vec4(lighting, texture(diffuseTexture, TextCoord).a);
}
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

uniform bool terrain;
uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

//Terrain
uniform sampler2D terrainTexture1;
uniform sampler2D terrainTexture2;

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
	float shadow = currentDepth - bias > closestDepth  ? 0.5 : 0.0;  

    return shadow;
}


void main () {
	vec3 color;
	if(!terrain)
		color = texture(texture_diffuse1, TextCoord).rgb;
	else{
		const float zone1 = -2.0;
		const float zone2 = -1.0;
		const float zone3 = 0.5;
		const float zone4 = 4.0;

		if(FragPos.y < zone1)
			color = texture(terrainTexture1, TextCoord).rgb;
		else if(FragPos.y > zone1 && FragPos.y < zone2 )
			color = mix(texture(terrainTexture1, TextCoord), texture(texture_diffuse1, TextCoord), 0.5).rgb;
		else if(FragPos.y > zone2 && FragPos.y < zone3 )
			color = texture(texture_diffuse1, TextCoord).rgb;
		else if(FragPos.y > zone3 && FragPos.y < zone4 )
			color = mix(texture(texture_diffuse1, TextCoord), texture(terrainTexture2, TextCoord), (FragPos.y - zone3)/(zone4-zone3)).rgb;
		else
			color = texture(terrainTexture2, TextCoord).rgb;
	}

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
	vec4 texel = vec4(lighting, texture(texture_diffuse1, TextCoord).a);
	if(texel.a < 0.5)
		discard;
	 frag_colour = texel;
}
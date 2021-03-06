#version 330 core

struct Light {					// Light information
 vec3 direction;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

struct Material {				// Object material information
 vec4 ambient;
 vec4 diffuse;
 vec4 specular;
 float shininess;
};

uniform bool terrain;			// Flag to do Terrain texture computations

uniform sampler2D texture_diffuse1;		// Main Texture
uniform sampler2D shadowMap;			// Shadow Map

//Terrain Textures
uniform sampler2D terrainTexture1;		
uniform sampler2D terrainTexture2;
uniform sampler2D terrainTexture3;
uniform sampler2D terrainTexture4;
uniform sampler2D terrainTexture5;
uniform sampler2D blendMap;

uniform Material material;				 // Material Uniform
uniform Light light;					// Light uniform
uniform vec3 viewPos;					// View Position uniform
in vec3 FragPos;						// Fragment Position 
in vec3 Normal;							// Normal

layout(location = 0) out vec4 frag_colour;	//final output color used to render the point

in vec4 ShadowCoord;
in vec2 TextCoord;


/*
	Calculates shadow value in a given point
*/
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
		vec2 tiledCoords = TextCoord * 300.0;			// Tiling for terrain Textures
		const float zone1 = -2.0;	// Terrain Zones
		const float zone2 = -1.0;	// Terrain Zones
		const float zone3 = 0.5;	// Terrain Zones
		const float zone4 = 4.0;	// Terrain Zones

		if(FragPos.y < zone1)
			color = texture(terrainTexture1, tiledCoords).rgb;
		else if(FragPos.y > zone1 && FragPos.y < zone2 )
			color = mix(texture(terrainTexture1, tiledCoords), texture(texture_diffuse1, tiledCoords), 0.5).rgb;
		else if(FragPos.y > zone2 && FragPos.y < zone3 ){


			vec3 blendMapColor = texture(blendMap, TextCoord).rgb;	
			
			float normaltexAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
			
			
			vec3 normalTex = texture(texture_diffuse1, tiledCoords).rgb * normaltexAmount;
			
			vec3 leavesTexColor = texture(terrainTexture3, tiledCoords).rgb * blendMapColor.r ;

			vec3 dirtTexColor =  texture(terrainTexture4, tiledCoords).rgb * blendMapColor.g ;

			vec3 pathTexColor =  texture(terrainTexture5, tiledCoords).rgb * blendMapColor.b ;
			color = normalTex + leavesTexColor + dirtTexColor + pathTexColor;
			//color = texture(texture_diffuse1, tiledCoords).rgb;
		
		}else if(FragPos.y > zone3 && FragPos.y < zone4 )
			color = mix(texture(texture_diffuse1, tiledCoords), texture(terrainTexture2, tiledCoords), (FragPos.y - zone3)/(zone4-zone3)).rgb;
		else
			color = texture(terrainTexture2, tiledCoords).rgb;
	}

	vec3 lightDir = normalize(light.direction);

	// Phong Model lighting

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
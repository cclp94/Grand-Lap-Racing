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

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
in vec3 FragPos;  
in vec3 Normal; 
in vec3 out_Color;
out vec4 frag_colour;	//final output color used to render the point

void main () {

	vec3 lightDir = normalize(FragPos - light.direction);


	// Ambient
	 vec4 ambient = light.ambient * material.ambient;
	
	 // Diffuse
	 vec3 norm = normalize(Normal);
	 float diff = max(dot(norm, lightDir), 0.0);
	 vec4 diffuse = light.diffuse * (diff * material.diffuse);
	
	 // Specular
	 vec3 viewDir = normalize(viewPos - FragPos);
	 vec3 reflectDir = reflect(-lightDir, norm);
	 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	 vec4 specular = light.specular * (spec * material.specular);
	
	 vec4 result = (ambient + diffuse + specular);
	 frag_colour = result;
}
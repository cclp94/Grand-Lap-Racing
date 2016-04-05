#version 330 core

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform vec3 vertex_color;
uniform mat4 lightSpaceMatrix;
uniform mat4 shadowBias;
uniform vec4 plane;

layout (location=0) in  vec3 in_Position;		//vertex position
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
out vec3 out_Color;
out vec3 Normal;
out vec3 FragPos;
out vec4 ShadowCoord;
out vec2 TextCoord;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM*vec4(in_Position, 1);

	

	FragPos = vec3(model_matrix * vec4(in_Position, 1.0f));
    Normal = transpose(inverse(mat3(model_matrix))) * normal;
	ShadowCoord =  lightSpaceMatrix * vec4(FragPos, 1.0f);
	TextCoord = texCoord;

	vec4 worldPosition = model_matrix * vec4(in_Position, 1.0f);

	gl_ClipDistance[0] = dot(worldPosition, plane);
}
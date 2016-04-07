#version 330 core

uniform mat4 view_matrix, model_matrix, proj_matrix;		// Transformation Matrices
uniform mat4 lightSpaceMatrix;								// Light View-Projection Matriz
uniform vec4 plane;											// Plane used for clip Distance

layout (location=0) in  vec3 in_Position;					//vertex position
layout (location=1) in vec3 normal;							// Normals of the mesh
layout (location=2) in vec2 texCoord;						// Texture Coordinates
out vec3 Normal;											// Normal sent to Fragment Shader
out vec3 FragPos;											// Vertex position in world space
out vec4 ShadowCoord;										// vertex Coordinates in shadow map
out vec2 TextCoord;											// Texture Coordinates sent to Frag Shader

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM*vec4(in_Position, 1);

	FragPos = vec3(model_matrix * vec4(in_Position, 1.0f));
    Normal = transpose(inverse(mat3(model_matrix))) * normal;
	ShadowCoord =  lightSpaceMatrix * vec4(FragPos, 1.0f);
	TextCoord = texCoord;

	vec4 worldPosition = model_matrix * vec4(in_Position, 1.0f);

	gl_ClipDistance[0] = dot(worldPosition, plane);			// Clips scene from a certain plane
}
#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform vec3 cameraPos;

layout (location=0) in  vec3 in_Position;		//vertex position

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 cameraVec;

const float tiling = 6.0;

void main () {

	vec4 worldPos = model_matrix * vec4(vec3(in_Position.x, 0.0, in_Position.z), 1); ;
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	clipSpace = CTM * vec4(vec3(in_Position.x, 0.0, in_Position.z), 1); 
	gl_Position = clipSpace;

	textureCoords = vec2(in_Position.x/2.0 + 0.5, in_Position.z/2.0 + 0.5) * tiling;
	
	cameraVec = normalize(cameraPos - worldPos.xyz);
}
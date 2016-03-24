#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;

layout (location=0) in  vec3 in_Position;		//vertex position

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM*vec4(in_Position, 1);
}
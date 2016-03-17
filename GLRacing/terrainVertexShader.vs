#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform vec3 vertex_color;


layout (location=0) in  vec3 in_Position;		//vertex position
layout (location=1) in vec3 normal;
out vec3 out_Color;
out vec3 Normal;
out vec3 FragPos;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM*vec4(in_Position, 1);

	FragPos = vec3(model_matrix * vec4(vec3(in_Position), 1.0f));
    Normal = normal;
}
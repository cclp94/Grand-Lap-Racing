#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform vec3 vertex_color;
uniform ivec2 HALF_TERRAIN_SIZE;
uniform sampler2D heightMapTexture;
uniform float scale;
uniform float half_scale;

in  vec3 in_Position;		//vertex position
out vec3 out_Color;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	float height = texture(heightMapTexture, in_Position.xz).r*scale - half_scale;
	vec2 pos  = (in_Position.xz*2.0-1)*HALF_TERRAIN_SIZE;
	gl_Position = CTM*vec4(pos.x, height, pos.y, 1);

	out_Color = vertex_color + vec3(height);
}
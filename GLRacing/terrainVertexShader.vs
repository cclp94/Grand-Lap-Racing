#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform vec3 vertex_color;
uniform ivec2 HALF_TERRAIN_SIZE;
uniform sampler2D heightMapTexture;
uniform sampler2D normalMapTexture;
uniform float scale;
uniform float half_scale;


in  vec3 in_Position;		//vertex position
out vec3 out_Color;
out vec3 Normal;
out vec3 FragPos;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	float height = texture(heightMapTexture, in_Position.xz).r*scale - half_scale;
	vec2 pos  = (in_Position.xz*2.0-1)*HALF_TERRAIN_SIZE;
	gl_Position = CTM*vec4(pos.x, height, pos.y, 1);

	FragPos = vec3(model_matrix * vec4(vec3(in_Position), 1.0f));

	vec3 normal = texture(normalMapTexture, in_Position.xz).rgb;
    // Transform normal vector to range [-1,1]
    Normal = normalize(normal * 2.0 - 1.0);
}
#version 330

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 cameraVec;

out vec4 frag_color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform float moveFactor;

const float waveStrength = 0.005;

void main () {

	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 +0.5;
	vec2 refractionTexCoord = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);

	vec2 distortedTexCoords = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0) -1.0;
	vec2 distortedTexCoords2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y+ moveFactor)).rg * 2.0) -1.0;
	vec2 finalDistortion = (distortedTexCoords+distortedTexCoords2) * waveStrength;

	refractionTexCoord += finalDistortion;
	reflectionTexCoord += finalDistortion;

	refractionTexCoord = clamp(refractionTexCoord, 0.001, 0.999);

	reflectionTexCoord.x = clamp(reflectionTexCoord.x, 0.001, 0.999);
	reflectionTexCoord.y = clamp(reflectionTexCoord.y, -0.999, 0.001);


	float refractiveFactor = dot(cameraVec, vec3(0.0, 1.0, 0.0));

	vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoord);
	vec4 refractionColor = texture(refractionTexture, refractionTexCoord);




	vec4 color = mix(reflectionColor, refractionColor, 0.8);
	frag_color = mix(color, vec4(0.0, 0.1, 0.3,0.7), 0.1);
}
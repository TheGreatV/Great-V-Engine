#version 330

uniform	vec3	materialAlbedo;
uniform	float	materialRoughness;
uniform	float	materialMetalness;

uniform	vec3	objectColor;

in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;


out vec4	oAlbedo;
out vec4	oNormal;
out vec4	oRoughnessMetalnessOcclusion;


void main()
{
	vec3 normal = normalize(fTBN[2]);
	
	vec3	albedo		= materialAlbedo;
	float	roughness	= materialRoughness;
	float	metalness	= materialMetalness;
	float	occlusion	= 1.0f;
	
	oAlbedo = vec4(albedo * objectColor, 1.0f);
	oNormal = vec4(normal, 1.0f);
	oRoughnessMetalnessOcclusion = vec4(roughness, metalness, occlusion, 1.0f);
}






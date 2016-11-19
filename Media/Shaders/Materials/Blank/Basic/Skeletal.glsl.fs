#version 330

uniform	vec3	materialColor;
uniform	vec3	materialSpecular;
uniform	float	materialGloss;
uniform	float	materialRoughness;


in	vec3	fPos;
in	mat3	fTBN;
in	vec2	fTex;
in	vec4	fTest;


out	vec4	oColor;
out	vec4	oSpecular;
out	vec4	oNormal;
out	vec4	oMaterial;


void main()
{
	vec3 normal = normalize(fTBN[2]);
	
	vec3 albedo		= mix(materialColor,	vec3(0.0f),			materialGloss);
	vec3 specular	= mix(vec3(0.0f),		materialSpecular,	materialGloss);
	
	oColor		= vec4(albedo, 1.0f);
	oSpecular	= vec4(specular, 1.0f);
	oNormal		= vec4(normal, 1.0f);
	oMaterial	= vec4(materialRoughness, 0.0f, 0.0f, 1.0f);
}
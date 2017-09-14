#version 330

uniform	vec3	materialAlbedo;
uniform	float	materialRoughness;
uniform	float	materialMetalness;

uniform	vec3	objectColor;

uniform	mat3	cameraRotateMatrix;

in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;


out vec4	oDistortion;


void main()
{
	vec3	normal = normalize(fTBN[2]);

	float	px = dot(cameraRotateMatrix[0], normal);
	float	py = dot(cameraRotateMatrix[1], normal);
	
	vec2	distortion = vec2(px, py);
	
	oDistortion = vec4(distortion * 0.5f + 0.5f, 0.0f, 1.0f);
}






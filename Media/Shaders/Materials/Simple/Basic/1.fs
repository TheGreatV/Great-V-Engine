#version 330

uniform sampler2D textureColor;
uniform sampler2D textureNormals;
uniform sampler2D textureSpecular;

in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;

out vec4	oColor;
out vec4	oSpecular;
out vec4	oNormal;
out vec4	oMaterial;

void main()
{
	vec4 texColor = texture(textureColor, fTex);
	vec4 texNormals = texture(textureNormals, fTex);
	vec4 texSecular = texture(textureSpecular, fTex);
	
	float	occlusion = 1.0f; // texSecular.w;
	float	gloss = texSecular.x * 0.5f;
	float	roughness = texSecular.y;
	float	metallic = texSecular.z * 0.5f;
	
	// vec3 diffuse = vec3(1.0f);
	vec3 diffuse = mix(texColor.xyz * occlusion, vec3(0.0f), gloss);
	// vec3 specular = vec3(1.0f);
	vec3 specular = mix(vec3(0.0f), mix(vec3(1.0f), diffuse, metallic), gloss);
	vec3 position = fPos;
	// vec3 normal = normalize(fTBN[2]);
	vec3 normal = normalize(fTBN * (texNormals.xyz*2.0f - 1.0f));
	vec3 view = normalize(position);
	vec3 reflection = reflect(view, normal);
	
	oColor = vec4(diffuse, 1.0f);
	oSpecular = vec4(specular, 1.0f);
	oNormal = vec4(normal, 1.0f);
	oMaterial = vec4(roughness, 0.0f, 0.0f, 1.0f);
}
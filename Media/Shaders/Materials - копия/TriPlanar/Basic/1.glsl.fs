#version 330

uniform sampler2D textureColor;
uniform sampler2D textureNormals;
uniform sampler2D textureSpecular;


uniform	vec3	cameraPosition;


in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;


out vec4	oColor;
out vec4	oSpecular;
out vec4	oNormal;
out vec4	oMaterial;


void main()
{
	vec3	tPos = (fPos + cameraPosition) * vec3(1.0f / 16.0f);
	vec3	tBlend = normalize(abs(fTBN[2]));
	float	b = tBlend.x + tBlend.y + tBlend.z; tBlend /= b;
	vec2	texXY = tPos.xy;
	vec2	texXZ = tPos.xz;
	vec2	texYZ = tPos.yz;
	

	vec4 texColor	= texture(textureColor, texYZ) * tBlend.x + texture(textureColor, texXZ) * tBlend.y + texture(textureColor, texXY) * tBlend.z;
	vec4 texNormals	= texture(textureNormals, texYZ) * tBlend.x + texture(textureNormals, texXZ) * tBlend.y + texture(textureNormals, texXY) * tBlend.z;
	vec4 texSecular	= texture(textureSpecular, texYZ) * tBlend.x + texture(textureSpecular, texXZ) * tBlend.y + texture(textureSpecular, texXY) * tBlend.z;
	// vec4 texColor = texture(textureColor, fTex);
	// vec4 texNormals = texture(textureNormals, fTex);
	// vec4 texSecular = texture(textureSpecular, fTex);
	
	float	occlusion = 1.0f; // texSecular.w;
	float	gloss = texSecular.x * 0.5f;
	float	roughness = texSecular.y;
	float	metallic = texSecular.z;
	
	vec3 diffuse = mix(texColor.xyz * occlusion, vec3(0.0f), gloss);
	vec3 specular = mix(vec3(0.0f), mix(vec3(1.0f), diffuse, metallic), gloss);
	vec3 position = fPos;
	vec3 normal = normalize(fTBN * (texNormals.xyz*2.0f - 1.0f));
	vec3 view = normalize(position);
	vec3 reflection = reflect(view, normal);
	
	oColor = vec4(diffuse, 1.0f);
	oSpecular = vec4(specular, 1.0f);
	oNormal = vec4(normal, 1.0f);
	oMaterial = vec4(roughness, 0.0f, 0.0f, 1.0f);
}
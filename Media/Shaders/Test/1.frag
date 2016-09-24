#version 330

uniform sampler2D textureColor;
uniform sampler2D textureNormals;
uniform sampler2D textureSpecular;

in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;

out vec4	oColor;

void main()
{
	vec3 lightPos = vec3(10.0f, 20.0f, -30.0f);


	vec4 texColor = texture(textureColor, fTex);
	vec4 texNormals = texture(textureNormals, fTex);
	vec4 texSecular = texture(textureSpecular, fTex);
	
	
	vec3 position = fPos;
	vec3 view = normalize(fPos);
	vec3 normal = normalize(fTBN * (texNormals.xyz*2.0f - 1.0f));
	vec3 reflectView = reflect(view, normal);
	
	
	vec3 lightDif = lightPos - position;
	vec3 lightDir = normalize(lightDif);
	
	float diffuseIntensity = dot(normal, lightDir);
	
	float specularIntensity = pow( max(dot(reflectView, lightDir), 0.0f), 120.0f) * texSecular.x;

	
	oColor = vec4(texColor.xyz * (diffuseIntensity + specularIntensity), 1.0f);
}
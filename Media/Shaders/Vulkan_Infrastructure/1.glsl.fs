#version 450


layout(set = 0, binding = 1)	uniform	sampler2D	textureColor;
layout(set = 0, binding = 2)	uniform	sampler2D	textureNormals;
layout(set = 0, binding = 3)	uniform	sampler2D	textureSpecular;


layout(location = 0)	in	vec3		fPos;
layout(location = 1)	in	mat3		fTBN;
layout(location = 4)	in	vec2		fTex;


layout(location = 0)	out	vec4	oColor;


void main()
{
	vec3 lightPos = vec3(10.0f, 20.0f, -30.0f);

	
	vec4 texColor = texture(textureColor, fTex);
	vec4 texNormals = texture(textureNormals, fTex);
	vec4 texSecular = texture(textureSpecular, fTex);
	

	vec3 diffuse = texColor.xyz;
	vec3 position = fPos;
	vec3 view = normalize(fPos);
	vec3 normal = normalize(fTBN * (texNormals.xyz * 2.0f - 1.0f));
	vec3 reflectView = reflect(view, normal);
    
	
	vec3 lightDif = lightPos - position;
	vec3 lightDir = normalize(lightDif);
	
	float diffuseIntensity = max(dot(normal, lightDir), 0.0f);
	
	float specularIntensity = pow( max(dot(reflectView, lightDir), 0.0f), 128.0f) * texSecular.x;
	

	oColor = vec4(diffuseIntensity * diffuse + specularIntensity*vec3(1.0f), 1.0f);
	// oColor = vec4(fTex, 0.0f, 1.0f);
}
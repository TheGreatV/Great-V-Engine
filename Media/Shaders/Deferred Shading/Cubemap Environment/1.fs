#version 330 core

#define PI 3.1415926535897932384626433832795


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureColor;
uniform sampler2D	textureSpecular;
uniform sampler2D	textureNormal;
uniform sampler2D	textureMaterial;
uniform sampler2D	textureDepth;
uniform sampler2D	textureDiffuse;
uniform samplerCube	textureEnvironment;


uniform	vec2	screen;

uniform float	camFar;
uniform float	camFarMNear;
uniform float	camFarXNear;

uniform float	environmentMipmapsCount;
uniform vec3	environmentColor;
uniform	mat4	environmentInverseMatrix;


in	vec4	pRec;


out	vec4	oSpecular;


float Fresnel(vec3 normal, vec3 view, float roughness);
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light);
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness);
float Specular(vec3 normal, vec3 light, vec3 view, float roughness);


void main()
{
	vec2 screenTex = gl_FragCoord.xy/screen;

	vec4	dataColor = texture(textureColor, screenTex);
	vec4	dataSpecular = texture(textureSpecular, screenTex);
	vec4	dataNormal = texture(textureNormal, screenTex);
	vec4	dataMaterial = texture(textureMaterial, screenTex);
	vec4	dataDepth = texture(textureDepth, screenTex);
	vec4	dataDiffuse = texture(textureDiffuse, screenTex);

	vec3	color = dataColor.xyz;
	vec3	specular = dataSpecular.xyz;
	vec3	normal = dataNormal.xyz;
	float	roughness = dataMaterial.x;
	vec3	diffuse = dataDiffuse.xyz;
	float	diffuseIntensity = dataDiffuse.w;
	float	depth = dataDepth.x; if(depth >= 1.0f) return;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	vec3	position = (pRec.xyz*gl_FragCoord.w) * distance;
	vec3	view = -normalize(position); // from pixel to camera
	vec3	reflection = reflect(-view, normal);
	vec3	light = reflection; // from pixel to light
	
	vec3	offset = (environmentInverseMatrix * vec4(position, 1.0f)).xyz;
	vec3	offsetInverse = clamp(1.0f - abs(offset*offset), vec3(0.0f), vec3(1.0f));
	float	rangeIntensity = 1.0f; // offsetInverse.x*offsetInverse.y*offsetInverse.z;
	
	// float	specularIntensity = clamp(Specular(normal, light, view, roughness), 0.0f, 1.0f);
	float	specularIntensity = clamp(Fresnel(normal, view, roughness), 0.0f, 1.0f);
	
	float	mipmapLevel = pow(roughness, 1.0f)*environmentMipmapsCount;
	vec4	dataEnvironment = textureLod(textureEnvironment, reflection, mipmapLevel);
	vec3	environment = dataEnvironment.xyz;
	
	// oSpecular = vec4(vec3(specularIntensity), 1.0f);
	oSpecular = vec4(rangeIntensity*diffuseIntensity*specularIntensity * environment*environmentColor*specular, 1.0f);
}

float Fresnel(vec3 normal, vec3 view, float roughness) {
	float	r = roughness + 0.01f;
	
	float	A = 1.0f + 5.0f * r; // 5.0f
	float	B = (1.0f - pow(r, 2.0f)) / 3.0f;
	
	float	fresnel = 1.0f / pow(1.0f + pow(dot(view, normal), 2.0), A);
	float	schlick = pow(1.0f - pow(dot(view, normal), 2.0), A) * (1.0f - B) + B;
	
	return	fresnel;
}
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light) {
	vec3	half = normalize(view + light);
	
	float	implicit = dot(normal, light) * dot(normal, view);
	float	neumann = (dot(normal, light) * dot(normal, view)) / max(dot(normal, light), dot(normal, view));
	float	cookTorrance = min(	1.0f, min(
								(2.0f * dot(normal, half) * dot(normal, view)) / dot(view, half),
								(2.0f * dot(normal, half) * dot(normal, light)) / dot(view, half)));
	float	kelemen = (dot(normal, light) * dot(normal, view)) / pow(dot(view, half), 2.0f);
	
	return	kelemen;
}
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness) {
	float	r = roughness + 0.01f;
	vec3	half = normalize(view + light);
	float	r2 = pow(r, 2.0f);
	
	float	blinnPhong = (1.0f / (PI * r2)) * pow(dot(normal, half), (2.0f / r2) - 2.0f);
	float	beckmann = (1.0f / (PI * r2 * pow(dot(normal, half), 4.0f))) * exp((pow(dot(normal, half), 2.0f) - 1.0f) / (r2 * pow(dot(normal, half), 2.0f)));
	float 	trowbridgeReitz = r2 / (PI * pow(pow(dot(normal, half), 2.0f) * (r2 - 1.0f) + 1.0f, 2.0f));
	
	return	beckmann;
}
float Specular(vec3 normal, vec3 light, vec3 view, float roughness) { // +light -view
	float	distribution = Distribution(normal, view, light, roughness);
	float	fresnel = Fresnel(normal, view, roughness);
	float	geometricAttenuation = GeometricAttenuation(normal, view, light);

	
	float	blinn = pow(dot(reflect(-view, normal), -light), 1.01f / (roughness + 0.01f));
	float	cookTorrance = (distribution * fresnel * geometricAttenuation) / (dot(normal, view) * dot(normal, light));
	
	return	clamp(cookTorrance, 0.0f, 1.0f);
}

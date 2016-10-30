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
uniform vec4	environmentColor;


in	vec4	pRec;


out	vec4	oSpecular;


float Fresnel(vec3 normal, vec3 view, float roughness);


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
	
	float	totalRangeIntensity = environmentColor.w;
	
	// float	specularIntensity = clamp(Specular(normal, light, view, roughness), 0.0f, 1.0f);
	float	specularIntensity = clamp(Fresnel(normal, view, roughness), 0.0f, 1.0f);
	
	float	mipmapLevel = pow(roughness, 1.0f)*environmentMipmapsCount;
	vec4	dataEnvironment = textureLod(textureEnvironment, reflection, mipmapLevel);
	vec3	environment = dataEnvironment.xyz;
	
	// oSpecular = vec4(vec3(totalRangeIntensity), 1.0f);
	oSpecular = vec4(totalRangeIntensity*diffuseIntensity*specularIntensity * environment*environmentColor.xyz*specular, totalRangeIntensity);
}

float Fresnel(vec3 normal, vec3 view, float roughness) {
	float	r = roughness + 0.01f;
	
	float	A = 1.0f + 5.0f * r; // 5.0f
	float	B = (1.0f - pow(r, 2.0f)) / 3.0f;
	
	float	fresnel = 1.0f / pow(1.0f + pow(dot(view, normal), 2.0), A);
	float	schlick = pow(1.0f - pow(dot(view, normal), 2.0), A) * (1.0f - B) + B;
	
	return	fresnel;
}

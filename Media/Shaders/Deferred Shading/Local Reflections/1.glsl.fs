#version 330 core

#define PI 3.1415926535897932384626433832795
#define ITERATIONS_COUNT 32
#define INITIAL_FACTOR 5.0f
#define MAX_CAST_DISTANCE 15.0f


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
uniform mat4	viewProjectionMatrix;
uniform mat4	viewProjectionInverseMatrix;


in	vec4	pRec;


out	vec4	oSpecular;


float	Fresnel(vec3 normal, vec3 view, float roughness);
float	GetDistance(vec2 screenTex) {
	vec4	dataDepth = texture(textureDepth, screenTex);

	float	depth = dataDepth.x;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	
	return	distance;
}
float	GetDistance(vec3 position) {
	vec4	projectionCoord = viewProjectionMatrix * vec4(position, 1.0f);
	float	depth = (projectionCoord.z / projectionCoord.w) * 0.5f + 0.5f;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	
	return	distance;
}
vec3	GetPosition(vec2 screenTex, float distance) {
	vec3	screenCoord = vec3(screenTex * 2.0f - 1.0f, 1.0f);
	vec4	position = viewProjectionInverseMatrix * vec4(screenCoord, 1.0f);
	
	return	position.xyz * distance;
}
vec2	GetScreenCoord(vec3 position) {
	vec4	projectionCoord = viewProjectionMatrix * vec4(position, 1.0f);
	vec2	screenCoord = (projectionCoord.xy / projectionCoord.w) * 0.5f + 0.5f;
	
	return	screenCoord;
}


void main()
{
	vec2	screenTex = gl_FragCoord.xy/screen;

	vec4	dataSpecular = texture(textureSpecular, screenTex);
	vec4	dataMaterial = texture(textureMaterial, screenTex);
	vec4	dataDiffuse = texture(textureDiffuse, screenTex);

	vec3	specular = dataSpecular.xyz;
	float	roughness = dataMaterial.x;
	float	diffuseIntensity = dataDiffuse.w;
	
	
	vec2	tTex = screenTex;
	vec4	tDataNormal = texture(textureNormal, tTex);
	vec3	tNormal = normalize(tDataNormal.xyz);
	float	tDistance = GetDistance(tTex);
	vec3	tPosition = GetPosition(tTex, tDistance);
	vec3	tView = normalize(tPosition);
	vec3	tReflection = reflect(tView, tNormal);
	
	float	factor = INITIAL_FACTOR;
	vec3	probePosition = tPosition;
	vec2	nTex;
	vec3	nPosition;
	
	bool	isHit = false;
	for(int i = 1; i < ITERATIONS_COUNT; ++i) {
		nPosition = probePosition + tReflection * factor;
		nTex	= GetScreenCoord(nPosition);
		
		float	nDistance = GetDistance(nPosition);
		float	pDistance = GetDistance(nTex);
		
		if(nTex.x < 0.0f || nTex.x > 1.0f || nTex.y < 0.0f || nTex.y > 1.0f) {
			break ;
		}
		if(pDistance < nDistance) {
			factor *= 0.5f;
			isHit = true;
		}
		else {
			probePosition = nPosition;
		}
	}
	
	if(isHit) {
		float	fresnel = Fresnel(tNormal, tView, 0.1f);
		float	distanceSaturation = 1.0f - clamp(length(nPosition - tPosition) / MAX_CAST_DISTANCE, 0.0f, 1.0f);
		float	reflectionIntensity = distanceSaturation * fresnel;
		vec4	dataDiffuse = texture(textureDiffuse, nTex);
		vec3	diffuse = dataDiffuse.xyz;
		
		oSpecular = vec4(diffuse * reflectionIntensity*specular*diffuseIntensity, reflectionIntensity);
	}
	else {
		oSpecular = vec4(vec3(0.0f), 1.0f);
	}
}

float Fresnel(vec3 normal, vec3 view, float roughness) {
	float	r = roughness + 0.01f;
	
	float	A = 1.0f + 5.0f * r; // 5.0f
	float	B = (1.0f - pow(r, 2.0f)) / 3.0f;
	
	float	fresnel = 1.0f / pow(1.0f + pow(dot(view, normal), 2.0), A);
	float	schlick = pow(1.0f - pow(dot(view, normal), 2.0), A) * (1.0f - B) + B;
	
	return	fresnel;
}









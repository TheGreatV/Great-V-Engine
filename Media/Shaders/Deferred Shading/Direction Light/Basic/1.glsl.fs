#version 330 core

#define PI 3.1415926535897932384626433832795


uniform sampler2D	textureColor;
uniform sampler2D	textureSpecular;
uniform sampler2D	textureNormal;
uniform sampler2D	textureMaterial;
uniform sampler2D	textureDepth;


uniform float	camFar;
uniform float	camFarMNear;
uniform float	camFarXNear;

uniform vec3	lightDirection;
uniform vec3	lightColor;
uniform float	lightAmbient;
uniform float	lightFog; // TODO


in	vec4	pView;
in	vec2	pTex;


out	vec4	oDiffuse;
out	vec4	oSpecular;


float Fresnel(vec3 normal, vec3 view, float roughness);
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light);
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness);
float Diffuse(vec3 normal, vec3 light, vec3 view, float roughness, float ambient);
float Specular(vec3 normal, vec3 light, vec3 view, float roughness);


void main() {
	vec4 dataColor = texture(textureColor, pTex);
	vec4 dataSpecular = texture(textureSpecular, pTex);
	vec4 dataNormal = texture(textureNormal, pTex);
	vec4 dataMaterial = texture(textureMaterial, pTex);
	vec4 dataDepth = texture(textureDepth, pTex);

	vec3	color = dataColor.xyz;
	vec3	specular = dataSpecular.xyz;
	vec3	normal = dataNormal.xyz;
	float	roughness = dataMaterial.x;
	float	depth = dataDepth.x; if(depth >= 1.0f) return;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	vec3	position = pView.xyz * distance;
	vec3	view = -normalize(position); // from pixel to camera
	vec3	reflection = reflect(view, normal);
	vec3	light = -lightDirection; // from pixel to light
	
	float	diffuseIntensity = Diffuse(normal, light, view, roughness, lightAmbient);
	
	float	specularIntensity = Specular(normal, light, view, roughness);
	
	oDiffuse = vec4(diffuseIntensity * color*lightColor, diffuseIntensity);
	oSpecular = vec4(specularIntensity * specular*lightColor, 1.0f);
	// oColor = vec4((color + specularIntensity*specular)*diffuseIntensity*lightColor ,1.0f);
	// oColor = pView;
	// oColor = vec4(vec3(specularIntensity), 1.0f);
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
float Diffuse(vec3 normal, vec3 light, vec3 view, float roughness, float ambient) {
	float r = pow(roughness, 2.0f);
	float A = 1.0f - 0.5f * (r / (r + 0.33f));
	float B = 0.45f * (r / (r + 0.09f));
	float a = max( dot(normal, light), dot(normal, view) );
	float b = min( dot(normal, light), dot(normal, view) );
	float fL = acos(dot(normal, light));
	float fV = acos(dot(normal, view));
	
	float lambert = max(dot(normal, light), ambient);
	float orenNayar = ambient + (1.0f - ambient) * max(dot(normal, light) * (A + B * max(0.0f, cos(fV - fL)) * sin(a) * tan(b)), 0.0f);
	
	return lambert;
}
float Specular(vec3 normal, vec3 light, vec3 view, float roughness) { // pixel->light, pixel->camera
	float	distribution = Distribution(normal, view, light, roughness);
	float	fresnel = Fresnel(normal, view, roughness);
	float	geometricAttenuation = GeometricAttenuation(normal, view, light);

	
	float p = 1.01f / pow(roughness + 0.01f, 2.0f);
	
	float	phong = pow(max(dot(reflect(-light, normal), view), 0.0f), p) * fresnel;
	float	blinn = pow(max(dot(normal, normalize(light + view)), 0.0f), p) * fresnel;
	float	cookTorrance = max(
		pow(max(dot(light, normal) ,0.0f), 2.0f) * // just to cover artifacts of high roughness
		(distribution * fresnel * geometricAttenuation) /
		(dot(normal, view) * dot(normal, light)),
		0.0f);
	
	return	cookTorrance;
}



















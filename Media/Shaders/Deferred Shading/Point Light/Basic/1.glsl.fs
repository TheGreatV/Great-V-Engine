#version 330 core

#define PI 3.1415926535897932384626433832795


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureColor;
uniform sampler2D	textureSpecular;
uniform sampler2D	textureNormal;
uniform sampler2D	textureMaterial;
uniform sampler2D	textureDepth;


uniform	vec2	screen;

uniform float	camFar;
uniform float	camFarMNear;
uniform float	camFarXNear;

uniform vec3	lightPosition;
uniform vec3	lightColor;
uniform vec3	lightRange; // near, 1/(far-near)
uniform float	lightAmbient;
uniform float	lightFog; // TODO


in	vec4	pView;


out	vec4	oDiffuse;
out	vec4	oSpecular;


float Fresnel(vec3 normal, vec3 view, float roughness);
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light);
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness);
float Diffuse(vec3 normal, vec3 light, vec3 view, float roughness, float ambient);
float Specular(vec3 normal, vec3 light, vec3 view, float roughness);


void main() {
	vec2	screenTex = gl_FragCoord.xy/screen;

	vec4	dataColor = texture(textureColor, screenTex);
	vec4	dataSpecular = texture(textureSpecular, screenTex);
	vec4	dataNormal = texture(textureNormal, screenTex);
	vec4	dataMaterial = texture(textureMaterial, screenTex);
	vec4	dataDepth = texture(textureDepth, screenTex);

	vec3	color = dataColor.xyz;
	vec3	specular = dataSpecular.xyz;
	vec3	normal = dataNormal.xyz;
	float	roughness = dataMaterial.x;
	float	depth = dataDepth.x; if(depth >= 1.0f) return;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	vec3	position = (pView.xyz*gl_FragCoord.w) * distance; // *gl_FragCoord.w - perspective correction
	vec3	view = -normalize(position); // from pixel to camera

	vec3	lightOffset = position - lightPosition;
	vec3	lightDirection = normalize(lightOffset);
	vec3	light = -lightDirection; // from pixel to light
	
	float	rangeIntensity = 1.0f - clamp( (length(lightOffset) - lightRange.x) / (lightRange.y - lightRange.x), 0.0f, 1.0f);
	float	diffuseIntensity = Diffuse(normal, light, view, roughness, lightAmbient);
	float	specularIntensity = Specular(normal, light, view, roughness);

	oDiffuse = vec4(diffuseIntensity*rangeIntensity * color*lightColor, diffuseIntensity*rangeIntensity);
	oSpecular = vec4(specularIntensity*rangeIntensity * specular*lightColor, 1.0f);
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
float Specular(vec3 normal, vec3 light, vec3 view, float roughness) { // +light -view
	float	distribution = Distribution(normal, view, light, roughness);
	float	fresnel = Fresnel(normal, view, roughness);
	float	geometricAttenuation = GeometricAttenuation(normal, view, light);

	
	float p = 1.01f / pow(roughness + 0.01f, 2.0f);
	
	float	phong = pow(max(dot(reflect(-light, normal), view), 0.0f), p) * fresnel;
	float	blinn = pow(max(dot(normal, normalize(light + view)), 0.0f), p); // l+v / |l+v|
	float	cookTorrance = max(
		pow(max(dot(light, normal) ,0.0f), 2.0f) * // just to cover artifacts of high roughness
		(distribution * fresnel * geometricAttenuation) /
		(dot(normal, view) * dot(normal, light)),
		0.0f);
	
	return	cookTorrance;
}


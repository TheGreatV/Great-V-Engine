#version 420

#define PARALLAX_MAPPING 0

uniform mat4	viewProjectionMatrix;
uniform	vec3	cameraPosition;

uniform sampler2D textureAlbedo;
uniform sampler2D textureNormals;
uniform sampler2D textureHeight;
uniform sampler2D textureOcclusion;
uniform sampler2D textureRoughness;
uniform sampler2D textureMetalness;

in vec3		fPos;
in mat3		fTBN;
in vec2		fTex;

out vec4	oAlbedo;
out vec4	oNormal;
out vec4	oRoughnessMetalnessOcclusion;

void main()
{
#if PARALLAX_MAPPING
	vec3	view = normalize(fPos);
	
	float	stepsCount = 32.0f;
	float	stepFactor = 1.0f / stepsCount;
	float	emboss = 2.0f;
	float	scale = 20.0f; // one tile is scaled for 20 units
	float	embossScaleFactor = emboss / scale; // 0.1f;
	
	vec2	scanDirectionOnMap = vec2( dot(fTBN[0], view) , dot(fTBN[1], view) );
	vec2	scanStepOnMap = embossScaleFactor * stepFactor * scanDirectionOnMap; // 1 step must scan 'stepFactor' part of 'emboss' and corresponding part of 
	float	scanStepOnHeight = stepFactor;

	vec2	tex = fTex;
	vec2	previousTex = tex;
	
	// find first intersection (steep parallax mapping)
	float	scanHeight = 1.0f;
	float	currentHeight = texture(textureHeight, tex).x;
	while(scanHeight > currentHeight) {
		scanHeight -= scanStepOnHeight;
		previousTex = tex;
		tex += scanStepOnMap;
		currentHeight = texture(textureHeight, tex).x;
	}
	
	float	previousHeight = texture(textureHeight, previousTex).x;
	
	float	a = previousHeight, b = currentHeight, x = scanHeight + scanStepOnHeight, y = scanHeight;
	float	t = (a - x) / ((y - x) - (b - a));
	tex = mix(previousTex, tex, t);

	// parallax corrected depth
	float	th = 1.0f - mix(a, b, t); {
		vec3	pos = fPos + cameraPosition;
		vec3	d = fTBN * vec3(scanDirectionOnMap.x * embossScaleFactor, scanDirectionOnMap.y * embossScaleFactor, emboss);
		pos = pos - d * th;
		
		// oColor = vec4(vec3(pos.y), 1.0f);
		
		vec4	v = viewProjectionMatrix * vec4(pos, 1.0f);
		gl_FragDepth = v.z / v.w * 0.5f + 0.5f;
	}	
#else
	vec2	tex = fTex;
#endif
	
	vec4	dataAlbedo			= texture(textureAlbedo,	tex);
	vec4	dataNormals			= texture(textureNormals,	tex);
	vec4	dataHeight			= texture(textureHeight,	tex);
	vec4	dataOcclusion		= texture(textureOcclusion,	tex);
	vec4	dataRoughness		= texture(textureRoughness,	tex);
	vec4	dataMetalness		= texture(textureMetalness,	tex);
	
	
	vec3	albedo		= dataAlbedo.xyz;
	vec3	normal		= normalize(fTBN * (dataNormals.xyz * 2.0f - 1.0f));
	float	roughness	= dataRoughness.x;
	float	metalness	= dataMetalness.x;
	float	occlusion	= dataOcclusion.x;
	
	float	gloss		= 0.5f * (1.0f - roughness); // TODO: remove
	vec3	color		= albedo * occlusion; // TODO: remove
	vec3	specular	= mix(vec3(1.0f), albedo, metalness) * gloss * occlusion; // TODO: remove
	
	oAlbedo = vec4(albedo, 1.0f);
	oNormal = vec4(normal, 1.0f);
	oRoughnessMetalnessOcclusion = vec4(roughness, metalness, occlusion, 1.0f);
}


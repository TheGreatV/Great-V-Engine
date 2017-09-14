#version 330

#define PARALLAX_MAPPING 1

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

out vec4	oColor;
out vec4	oSpecular;
out vec4	oNormal;
out vec4	oMaterial;

void main()
{
#if PARALLAX_MAPPING
	vec3	view = normalize(fPos);
	
	float	stepsCount = 32.0f;
	float	stepsFactor = 1.0f / stepsCount;
	float	emboss = 2.0f;
	float	textureScale = 20.0f; // one tile is scaled for 20 units
	float	embossFactor = emboss / textureScale; // 0.1f;
	
	vec2	newTex = fTex;
	vec2	scanDirection = vec2( dot(fTBN[0], view) , dot(fTBN[1], view) );
	vec2	dTex = scanDirection * stepsFactor * embossFactor;
	
	// vec3	d = (dTex.x, , dTex.y);
	// dTex / embossFactor = ? * stepsFactor;
	
	float	scanHeight = 1.0f;
	float	height = texture(textureHeight, newTex).x;
	float	oldHeight = height;
	
	while( scanHeight > height ) {
		newTex += dTex;
		scanHeight -= stepsFactor;
		oldHeight = height;
		height = texture(textureHeight, newTex).x;
	}	
	
	newTex = newTex + dTex*( (scanHeight + stepsFactor-oldHeight) / (height - oldHeight + stepsFactor) - 1.0f );

	vec2	tex = newTex;
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
	float	occlusion	= dataOcclusion.x;
	float	roughness	= dataRoughness.x;
	float	metalness	= dataMetalness.x;
	
#if PARALLAX_MAPPING
	vec3	pos = fPos + cameraPosition;
	{
		// float	t = 1.0f - scanHeight;
		// vec3	d = fTBN * vec3(scanDirection.x * embossFactor, emboss, scanDirection.y * embossFactor);
		// pos = pos + d * t;
		
		// float	height = texture(textureHeight, tex).x;
		// pos	= pos + (height * emboss * fTBN[2]);

		// vec4	v = viewProjectionMatrix * vec4(pos, 1.0f);
		// gl_FragDepth = v.z / v.w * 0.5f + 0.5f;
	}
#endif
	
	float	gloss		= 0.5f * (1.0f - roughness); // TODO: remove
	vec3	color		= albedo * occlusion; // TODO: remove
	vec3	specular	= mix(vec3(1.0f), albedo, metalness) * gloss * occlusion; // TODO: remove
	
	oColor		= vec4(color, 1.0f);
	oSpecular	= vec4(specular, 1.0f);
	oNormal		= vec4(normal, 1.0f);
	oMaterial	= vec4(roughness, 0.0f, 0.0f, 1.0f);
}


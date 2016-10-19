#version 330 core


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform vec3		decalNormal;
uniform	mat3		rotateMatrix;
uniform mat4		modelInverseMatrix;

uniform	sampler2D	textureAlbedo;
uniform	sampler2D	textureTopology;
uniform	sampler2D	textureMaterial;
uniform	sampler2D	textureDepth;

uniform vec2		screen;

uniform float		camFar;
uniform float		camFarXNear;
uniform float		camFarMNear;


in	vec4	pRec;


out vec4	oColor;
out vec4	oSpecular;
out vec4	oNormal;
out vec4	oMaterial;


void main()
{
	vec2	screenTex = gl_FragCoord.xy/screen;

	vec4	dataDepth		= texture(textureDepth, screenTex);

	float	depth = dataDepth.x; if(depth >= 1.0f) return;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	vec3	position = (pRec.xyz*gl_FragCoord.w) * distance;
	
	vec3	projected = (modelInverseMatrix * vec4(position, 1.0f)).xyz;
	
	
	if(	min(min(projected.x, projected.y), projected.z) < 0.0f ||
		max(max(projected.x, projected.y), projected.z) > 1.0f)
	{
		return;
	}
	
	vec2	texCoord		= projected.xy;
	
	vec4	dataAlbedo		= texture(textureAlbedo, texCoord);
	vec4	dataTopology	= texture(textureTopology, texCoord);
	vec4	dataMaterial	= texture(textureMaterial, texCoord);

	
	float	intensity = dataAlbedo.w;
	float	gloss = 0.0f; // dataMaterial.x;
	float	roughness = dataMaterial.y;
	float	metallic = dataMaterial.z;
	float	occlusion = 1.0f; // dataMaterial.w;
	vec3	albedo = dataAlbedo.xyz; // vec3(projected.xy, 0.0f);
	vec3	diffuse = albedo * occlusion;
	vec3	color = mix(diffuse, vec3(0.0f), gloss);
	vec3	specular = mix(vec3(0.0f), mix(vec3(1.0f), diffuse, metallic), gloss); // vec3(0.0f);
	vec3	dataNormals = dataTopology.xyz * 2.0f - 1.0f;
	vec3	normal = rotateMatrix * (dataNormals * vec3(1,1,-1)); // decalNormal;
	vec3	material = vec3(roughness, 0.0f, 0.0f);
	
	float	intensityColor = intensity;
	float	intensitySpecular = intensity;
	float	intensityNormal = intensity;
	float	intensityMaterial = intensity;
	
	
	oColor		= vec4(color,		intensityColor);
	oSpecular	= vec4(specular,	intensitySpecular);
	oNormal		= vec4(normal,		intensityNormal);
	oMaterial	= vec4(material,	intensityMaterial);
}





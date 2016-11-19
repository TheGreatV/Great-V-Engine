#version 330 core

#define PI 3.1415926535897932384626433832795
#define SAMPLES_COUNT 16
#define CAST_DISTANCE 6.0f
#define CAST_BIAS 5.0f


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureDepth;
uniform sampler2D	textureNormal;


uniform	vec2	screen;
uniform	float	mipmapsCount;

uniform float	camFar;
uniform float	camFarMNear;
uniform float	camFarXNear;
uniform mat4	viewProjectionMatrix;
uniform mat4	viewProjectionInverseMatrix;


in	vec4	pView;


out	vec4	oOcclusion1;
out	vec4	oOcclusion2;


/*float	GetDistance(vec2 screenTex) {
	vec4	dataDepth = texture(textureDepth, screenTex);

	float	depth = dataDepth.x;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	
	return	distance;
}*/
float	GetDistance(vec3 position) {
	vec4	projectionCoord = viewProjectionMatrix * vec4(position, 1.0f);
	float	depth = (projectionCoord.z / projectionCoord.w) * 0.5f + 0.5f;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	
	return	distance;
}
vec2	GetScreenCoord(vec3 position) {
	vec4	projectionCoord = viewProjectionMatrix * vec4(position, 1.0f);
	vec2	screenCoord = (projectionCoord.xy / projectionCoord.w) * 0.5f + 0.5f;
	
	return	screenCoord;
}


void main()
{
	vec2	screenTex = gl_FragCoord.xy/screen;

	vec4	dataNormal = texture(textureNormal, screenTex);
	// vec4	dataDepth = texture(textureDepth, screenTex);

	vec3	normal = normalize(dataNormal.xyz);
	// float	depth = dataDepth.x; if(depth >= 1.0f) return;
	
	float	distance = texture(textureDepth, screenTex).x;
	// float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	
	vec3	position = pView.xyz * distance;
	
	vec3	nor = normal;
	vec3	tan = normalize(cross(nor, cross(nor, nor.zxy)));
	vec3	bin = normalize(cross(nor, tan));
	mat3	tbn = mat3(tan, bin, nor);
	
	float	occlusion = 1.0f;
	
	for(int x = 0; x < SAMPLES_COUNT; ++x) {
		for(int y = 1; y < SAMPLES_COUNT / 4; ++y) {
			for(int z = 1; z <= SAMPLES_COUNT / 8; ++z) {
				float	dx = float(x) / float(SAMPLES_COUNT);
				float	dy = float(y) / float(SAMPLES_COUNT / 4);
				float	tdz = float(z - 1) / float(SAMPLES_COUNT / 8 - 1);
				float	dz = float(z) / float(SAMPLES_COUNT / 8);
				
				vec3	point = vec3(
					sin(dy * PI / 2.0f) * sin(dx * PI * 2.0f),
					sin(dy * PI / 2.0f) * cos(dx * PI * 2.0f),
					cos(dy * PI / 2.0f)
					) * CAST_DISTANCE * (1.0f - pow(1.0f - dz, 2.0f));
				
				vec3	probePosition = position + tbn * point;
				float	probeDistance = GetDistance(probePosition);
				vec2	probeScreenCoord = GetScreenCoord(probePosition);
				
				float	screenDistance = textureLod(textureDepth, probeScreenCoord, tdz * mipmapsCount).x;
				// float	screenDistance = GetDistance(probeScreenCoord);
				
				// occlusion fade with distance
				float	factor = clamp((probeDistance - screenDistance) / CAST_BIAS, 0.0f, 2.0f);
				float	smoothFactor = 1.0f - abs(factor - 1.0f);
				
				// far probes affects less than near probes
				float	distanceFactor = float(z) * (1.0f / ((1.0f + (SAMPLES_COUNT / 8)) / 2.0f * (SAMPLES_COUNT / 8)));

				occlusion -= smoothFactor * distanceFactor * (1.0f / (SAMPLES_COUNT * (SAMPLES_COUNT / 4 - 1) * (SAMPLES_COUNT / 8)));
			}
		}
	}
	
	oOcclusion1 = vec4(vec3(0.0f), occlusion);
	oOcclusion2 = vec4(vec3(0.0f), occlusion);
}









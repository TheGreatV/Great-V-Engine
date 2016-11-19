#version 330 core

#define BLUR_RARIUS 5


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureDepth;


uniform	vec2	screen;

uniform float	camFar;
uniform float	camFarMNear;
uniform float	camFarXNear;
uniform mat4	viewProjectionMatrix;
uniform mat4	viewProjectionInverseMatrix;


in	vec4	pView;


out	vec4	oDistance;


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
vec2	GetScreenCoord(vec3 position) {
	vec4	projectionCoord = viewProjectionMatrix * vec4(position, 1.0f);
	vec2	screenCoord = (projectionCoord.xy / projectionCoord.w) * 0.5f + 0.5f;
	
	return	screenCoord;
}


void main()
{
	vec2	screenTex = gl_FragCoord.xy/screen;

	/*float	distance = 0.0f; 
	float	samples = 1.0f;

	
	for(int x = -BLUR_RARIUS; x <= BLUR_RARIUS; ++x) {
		for(int y = -BLUR_RARIUS; y <= BLUR_RARIUS; ++y) {
			float	sampleFactor = 1.0f - length(vec2(x, y)) / BLUR_RARIUS;
			
			if(sampleFactor > 0.0f) {
				vec2	sampleTex = screenTex + vec2(x, y) / screen;
				vec4	sampleDataDepth = texture(textureDepth, sampleTex);
				float	sampleDepth = sampleDataDepth.x;
				float	sampleDistance = camFarXNear / (camFar - sampleDepth * camFarMNear); 
				
				distance	+= sampleDistance * sampleFactor;
				samples		+= sampleFactor;
			}
		}
	}
	
	distance /= samples;*/

	vec4	dataDepth = texture(textureDepth, screenTex);
	float	depth = dataDepth.x;
	float	distance = camFarXNear / (camFar - depth * camFarMNear); 
	

	oDistance	= vec4(distance);
}









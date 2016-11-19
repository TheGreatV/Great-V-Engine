#version 330 core


#define INTENSITY_MIN 0.5f
#define INTENSITY_MAX 1.5f


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureDiffuse;
uniform sampler2D	textureSpecular;

uniform	vec2	screen;


in	vec4	pView;


out	vec4	oColor;


void main() {
	vec2	screenTex = gl_FragCoord.xy/screen;

	vec4	dataDiffuse = texture(textureDiffuse, screenTex);
	vec4	dataSpecular = texture(textureSpecular, screenTex);
	vec4	dataColor = dataDiffuse + dataSpecular;
	

	// float	intensity = clamp((length(dataColor.xyz) - INTENSITY_MIN) / (INTENSITY_MAX - INTENSITY_MIN), 0.0f, 1.0f);
	float	intensity = max((length(dataColor.xyz) / sqrt(3.0f) - INTENSITY_MIN) / (INTENSITY_MAX - INTENSITY_MIN), 0.0f);
	
	float	factor = pow(intensity, 2.0f);
	
	oColor	= vec4(dataColor.xyz * factor, 1.0f);
}









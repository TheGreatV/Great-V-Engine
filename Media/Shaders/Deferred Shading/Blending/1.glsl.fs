#version 330 core


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureDiffuse;
uniform sampler2D	textureSpecular;
uniform sampler2D	textureDistortion;


in	vec2	pTex;


out	vec4	oColor;


void main()
{
	vec4	distortionData = texture(textureDistortion, pTex);
	vec2	distortion = distortionData.xy * 2.0f - 1.0f;

	vec2	tex = pTex + distortion * 0.1f;
	
	vec4	diffuse		= texture(textureDiffuse, tex);
	vec4	specular	= texture(textureSpecular, tex);
	
	oColor	= vec4(diffuse.xyz + specular.xyz, 1.0f);
}









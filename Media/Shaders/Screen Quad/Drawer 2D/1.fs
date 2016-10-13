#version 330 core


uniform sampler2D	textureColor;


in	vec2	pTex;


out	vec4	oColor;


void main()
{
	oColor = texture(textureColor, pTex);
	// oColor = vec4(pTex,0,1);
	// oColor = vec4(1,1,1,1);
}

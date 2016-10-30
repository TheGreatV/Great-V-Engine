#version 450


layout(set = 0, binding = 1) uniform sampler2D textureColor;


in	vec3		fPos;
in	vec2		fTex;


layout(location = 0) out vec4 oColor;


void main()
{
	vec4 texColor = texture(textureColor, fTex);
	

	oColor = vec4(texColor.xyz, 1.0f);
}
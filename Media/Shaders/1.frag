#version 450

layout(set = 0, binding = 1) uniform sampler2D testTexture;

in vec2 fTex;

layout(location = 0) out vec4 oColor;

void main()
{
	vec4 t = texture(testTexture, fTex);
	oColor = t; //vec4(t.xyz, 1.0f);// vec4(vec3(fTex, 1.0f) * t.xyz, 1.0f);
}
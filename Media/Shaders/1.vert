#version 450

layout(set = 0, binding = 0, column_major) uniform uniformData
{
	mat4 modelMatrix;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTex;

out vec2 fTex;

void main()
{
	fTex = vTex;
	
	gl_Position = modelMatrix * vec4(vPos, 1.0f);
	// gl_Position.xyz /= gl_Position.w;
	gl_Position.y *= -1.0f;
}
#version 450


struct ModelStruct
{
	mat4	modelViewProjectionMatrix;
};

layout(set = 0, binding = 0, column_major) uniform ModelData
{
	ModelStruct modelData;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTex;


out	vec2		fTex;


void main()
{
	fTex = vTex;

	gl_Position = modelData.modelViewProjectionMatrix * vec4(vPos, 1.0f);
	gl_Position.y = -gl_Position.y;
}

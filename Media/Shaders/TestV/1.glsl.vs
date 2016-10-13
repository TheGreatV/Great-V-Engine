#version 450

struct ModelStruct
{
	mat4	modelViewProjectionMatrix;
	mat4	modelMatrix;
	mat3	rotateMatrix;
};

layout(set = 0, binding = 0, column_major) uniform ModelData
{
	ModelStruct modelData;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vTan;
layout(location = 2) in vec3 vBin;
layout(location = 3) in vec3 vNor;
layout(location = 4) in vec2 vTex;


out	vec3		fPos;
out	mat3		fTBN;
out	vec2		fTex;


void main()
{
	fPos = (modelData.modelMatrix * vec4(vPos, 1.0f)).xyz;
	fTBN = modelData.rotateMatrix * mat3(vTan, vBin, vNor);
	fTex = vTex;

	gl_Position = modelData.modelViewProjectionMatrix * vec4(vPos, 1.0f);
	gl_Position.y = -gl_Position.y;
	gl_Position.z = -gl_Position.z;
}

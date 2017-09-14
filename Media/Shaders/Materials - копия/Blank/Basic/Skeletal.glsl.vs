#version 330

#define	MAX_BONES_COUNT 128


struct BoneData {
	mat4	modelMatrix;
};


uniform mat4	viewProjectionMatrix;
uniform mat4	modelViewProjectionMatrix;
uniform mat4	modelMatrix;
uniform mat3	rotateMatrix;


layout(std140, column_major) uniform BonesData {
	BoneData	bonesData[MAX_BONES_COUNT];
};


in	vec3		vPos;
in	vec3		vTan;
in	vec3		vBin;
in	vec3		vNor;
in	vec2		vTex;
in	vec4		vInd;
in	vec4		vWei;


out	vec3		fPos;
out	mat3		fTBN;
out	vec2		fTex;
out	vec4		fTest;


mat3	GetRotate(mat4 mat) {
	return mat3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}

void main()
{
	// gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0f);
	
	vec4	weight = normalize(vWei); weight *= weight;
	
	gl_Position = viewProjectionMatrix * vec4(
		(bonesData[uint(vInd[0])].modelMatrix * vec4(vPos, 1.0f)).xyz * weight[0] +
		(bonesData[uint(vInd[1])].modelMatrix * vec4(vPos, 1.0f)).xyz * weight[1] +
		(bonesData[uint(vInd[2])].modelMatrix * vec4(vPos, 1.0f)).xyz * weight[2] +
		(bonesData[uint(vInd[3])].modelMatrix * vec4(vPos, 1.0f)).xyz * weight[3],
		1.0f);
	
	mat3	r1 = GetRotate(bonesData[uint(vInd[0])].modelMatrix);
	mat3	r2 = GetRotate(bonesData[uint(vInd[1])].modelMatrix);
	mat3	r3 = GetRotate(bonesData[uint(vInd[2])].modelMatrix);
	mat3	r4 = GetRotate(bonesData[uint(vInd[3])].modelMatrix);
	
	vec3	tan =
		(r1 * vTan) * weight[0] +
		(r2 * vTan) * weight[1] +
		(r3 * vTan) * weight[2] +
		(r4 * vTan) * weight[3];
	
	vec3	bin =
		(r1 * vBin) * weight[0] +
		(r2 * vBin) * weight[1] +
		(r3 * vBin) * weight[2] +
		(r4 * vBin) * weight[3];
		
	vec3	nor =
		(r1 * vNor) * weight[0] +
		(r2 * vNor) * weight[1] +
		(r3 * vNor) * weight[2] +
		(r4 * vNor) * weight[3];
		
	// fTest = vec4(vec3( float(uint(vInd[0])) * 0.1f ), 1.0f);
	
	fTBN = mat3(tan, bin, nor);
}

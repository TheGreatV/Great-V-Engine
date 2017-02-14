#version 330

uniform mat4	modelViewProjectionMatrix;
uniform mat4	modelMatrix;
uniform mat3	rotateMatrix;

in vec3			vPos;
in vec3			vTan;
in vec3			vBin;
in vec3			vNor;
in vec2			vTex;

out vec3		fPos;
out mat3		fTBN;
out vec2		fTex;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0f);
	
	fPos = (modelMatrix * vec4(vPos, 1.0f)).xyz;
	fTBN = rotateMatrix * mat3(vTan, vBin, vNor);
	fTex = vTex;
}

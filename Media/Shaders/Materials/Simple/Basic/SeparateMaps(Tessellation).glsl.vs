#version 420

uniform mat4	modelMatrix;
uniform mat3	rotateMatrix;
uniform mat4	viewProjectionMatrix;
uniform mat4	localViewProjectionMatrix;
uniform mat4	modelViewProjectionMatrix;

in vec3			vPos;
in vec3			vTan;
in vec3			vBin;
in vec3			vNor;
in vec2			vTex;

out mat3		tcTBN;
out vec2		tcTex;

void main()
{
	// gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0f);
	gl_Position = modelMatrix * vec4(vPos, 1.0f);
	
	tcTBN = rotateMatrix * mat3(vTan, vBin, vNor);
	tcTex = vTex;
}

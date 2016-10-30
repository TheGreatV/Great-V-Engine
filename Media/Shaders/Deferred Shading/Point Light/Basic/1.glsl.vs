#version 330 core


uniform vec3	lightPosition;
uniform vec3	lightColor;
uniform vec3	lightRange; // near, far
uniform float	lightAmbient;
uniform float	lightFog; // TODO


in	vec3	vPos;
in	vec3	vColor;
in	vec4	vParam; // near,far,amb


out	mat4	gMat;


mat4 makeModelMatrix(vec3 pos,float size);


void main()
{
	vec3 position = lightPosition; // vec3(0.0f, 0.0f, 0.0f);
	float size = lightRange.y; // 1.0f;

	gMat = makeModelMatrix(position, size);
}

mat4 makeModelMatrix(vec3 pos, float size)
{
	return transpose(mat4
	(
		vec4(size,	0.0f,	0.0f,	pos.x),
		vec4(0.0f,	size,	0.0f,	pos.y),
		vec4(0.0f,	0.0f,	size,	pos.z),
		vec4(0.0f,	0.0f,	0.0f,	1.0f)
	));
}

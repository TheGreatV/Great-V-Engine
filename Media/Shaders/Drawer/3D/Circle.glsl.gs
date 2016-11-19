#version 330 core

#define VERTICES_COUNT 65


layout (points) in;
layout (line_strip, max_vertices = VERTICES_COUNT) out;


uniform	mat4			viewProjectionMatrix;
uniform	mat3			rotateMatrix;
uniform vec3			p;
uniform float			s;
uniform vec4			c;


out	vec4	pColor;


void main()
{
	for(int i = 0; i < VERTICES_COUNT; ++i)
	{
		float a = radians(float(i) / float(VERTICES_COUNT - 1) * 360.0f);
		
		gl_Position = viewProjectionMatrix * vec4(
			p + rotateMatrix * vec3(s * vec2(sin(a), cos(a)), 0.0f),
			1.0f);
		
		pColor = c;
		EmitVertex();
	}

    EndPrimitive();																											
}


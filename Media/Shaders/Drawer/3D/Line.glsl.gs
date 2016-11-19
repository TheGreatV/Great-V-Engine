#version 330 core


layout (points) in;
layout (line_strip, max_vertices = 2) out;


uniform	mat4			viewProjectionMatrix;
uniform vec3			p1;
uniform vec3			p2;
uniform vec4			c1;
uniform vec4			c2;


out	vec4	pColor;


void main()
{
	gl_Position = viewProjectionMatrix * vec4(p1, 1.0f); pColor = c1; EmitVertex();
	gl_Position = viewProjectionMatrix * vec4(p2, 1.0f); pColor = c2; EmitVertex();
    EndPrimitive();																											
}


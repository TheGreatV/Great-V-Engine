#version 330 core


layout (points) in;
layout (line_strip, max_vertices = 2) out;


uniform vec2			p1;
uniform vec2			p2;
uniform vec4			c1;
uniform vec4			c2;


out	vec4	pColor;


void main()
{
	gl_Position = vec4(p1, 0.0f, 1.0f); pColor = c1; EmitVertex();
	gl_Position = vec4(p2, 0.0f, 1.0f); pColor = c2; EmitVertex();
    EndPrimitive();																											
}


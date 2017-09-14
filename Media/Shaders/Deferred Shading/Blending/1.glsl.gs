#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


out	vec2	pTex;


void main()
{
	gl_Position = vec4(-1,-1,0,1);	pTex = vec2(0.0f, 0.0f); EmitVertex();
	gl_Position = vec4(+1,-1,0,1);	pTex = vec2(1.0f, 0.0f); EmitVertex();
	gl_Position = vec4(-1,+1,0,1);	pTex = vec2(0.0f, 1.0f); EmitVertex();
	gl_Position = vec4(+1,+1,0,1);	pTex = vec2(1.0f, 1.0f); EmitVertex();

    EndPrimitive();
}


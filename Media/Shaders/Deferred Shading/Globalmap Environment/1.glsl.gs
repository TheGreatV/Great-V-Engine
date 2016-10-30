#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


uniform mat4			viewProjectionInverseMatrix;


out	vec4	pRec;


void main()
{
	gl_Position = vec4(-1,-1,0,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = vec4(+1,-1,0,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = vec4(-1,+1,0,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = vec4(+1,+1,0,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();

    EndPrimitive();
}


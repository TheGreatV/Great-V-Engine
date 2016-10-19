#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 24) out;


uniform mat4			environmentMatrix;
uniform mat4			viewProjectionMatrix;
uniform mat4			viewProjectionInverseMatrix;


out	vec4	pRec;


void main()
{
	gl_Position = environmentMatrix * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = environmentMatrix * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = environmentMatrix * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = environmentMatrix * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = environmentMatrix * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();

	gl_Position = environmentMatrix * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = environmentMatrix * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();

    EndPrimitive();
}


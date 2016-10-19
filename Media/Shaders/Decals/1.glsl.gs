#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 24) out;


uniform mat4	modelViewProjectionMatrix;
uniform mat4	viewProjectionInverseMatrix;


out	vec4	pRec;


void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();

	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();
}


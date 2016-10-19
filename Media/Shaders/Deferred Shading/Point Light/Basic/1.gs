#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 24) out;


uniform mat4			viewProjectionMatrix;
uniform mat4			viewProjectionInverseMatrix;


in	mat4	gMat[];


out	vec4	pRec;


void main()
{
	mat4 mat = transpose(gMat[0] * transpose(viewProjectionMatrix));

	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();

	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    EndPrimitive();

    EndPrimitive();
}


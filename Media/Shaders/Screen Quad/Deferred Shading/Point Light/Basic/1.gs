#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 24) out;


uniform mat4			viewProjectionMatrix;
uniform mat4			viewProjectionInverseMatrix;


in	mat4	gMat[];
/*in	vec3	gPos[];
in	vec3	gColor[];
in	vec3	gSpecColor[];
in	vec4	gParam[];*/


out	vec4	pRec;
/*out	vec3	pLightPos;
out	vec3	pLightColor;
out	vec3	pLightSpecColor;
out	vec3	pLightParam;*/


void main()
{
	mat4 mat = transpose(gMat[0] * transpose(viewProjectionMatrix));

	/*pLightPos = gPos[0];
	pLightColor = gColor[0];
	pLightSpecColor = gSpecColor[0];
	pLightParam = gParam[0].xyz;*/

	gl_Position = mat * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1,-1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,-1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();

	gl_Position = mat * vec4(-1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,-1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(-1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
	gl_Position = mat * vec4(+1,+1,+1,1);	pRec = viewProjectionInverseMatrix * vec4(gl_Position.xy,1.0f,gl_Position.w); EmitVertex();
    EndPrimitive();

    EndPrimitive();
}


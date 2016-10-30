#version 330 core


layout (points) in;
layout (triangle_strip, max_vertices = 24) out;


uniform mat4			viewProjectionMatrix;
uniform mat4			viewProjectionInverseMatrix;


in	mat4	gMat[];


out	vec4	pView;


void main()
{
	mat4 mat = viewProjectionMatrix * gMat[0];

	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();																											
																															
	gl_Position = mat * vec4(-1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, -1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();

	gl_Position = mat * vec4(-1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, -1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(-1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
	gl_Position = mat * vec4(+1.0f, +1.0f, +1.0f, 1.0f);	pView = viewProjectionInverseMatrix * gl_Position; EmitVertex();
    EndPrimitive();

    EndPrimitive();
}


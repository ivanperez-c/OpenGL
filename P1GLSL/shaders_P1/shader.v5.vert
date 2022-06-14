#version 330 core

in vec3 inPos;		


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 modelViewProj;

out vec3 vcolor;

void main()
{
	if (mod(gl_VertexID, 4) == 0)
		vcolor = vec3(1, 1, 1);
	else if (mod(gl_VertexID, 4) == 1)
		vcolor = vec3(0, 1, 1);
	else if (mod(gl_VertexID, 4) == 2)
		vcolor = vec3(0, 0, 1);
	else
		vcolor = vec3(1, 1, 0);

	gl_Position =  modelViewProj * vec4(inPos, 1.0);

}

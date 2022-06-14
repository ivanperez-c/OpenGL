#version 330 core

in vec3 inPos;	
in vec3 inNormal;
in vec3 inColor;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 vpos;
out vec3 vnormal;
out vec3 vcolor;


void main()
{
	vpos = (modelView * vec4(inPos, 1.0)).xyz;
	vnormal = normalize((normal * vec4(inNormal, 0.0)).xyz);

	vcolor = inColor;

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

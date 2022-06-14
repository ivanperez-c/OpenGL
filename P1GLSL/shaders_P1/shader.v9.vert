#version 330 core

in vec3 inPos;		
in vec3 inNormal;	

uniform mat4 modelViewProj;

out vec3 vnormal;

void main()
{

	gl_Position =  modelViewProj * vec4(inPos, 1.0);
	vnormal = inNormal;
}

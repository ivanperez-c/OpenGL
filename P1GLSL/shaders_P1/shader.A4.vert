#version 330 core

in vec3 inPos;		
in vec3 inNormal;	
in vec2 inTexCoord;	

uniform mat4 modelViewProj;

out vec3 vnormal;
out vec2 vTexCoord;

void main()
{
	vTexCoord = inTexCoord;
	gl_Position =  modelViewProj * vec4(inPos, 1.0);
	vnormal = inNormal;
}

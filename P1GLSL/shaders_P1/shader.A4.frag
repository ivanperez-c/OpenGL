#version 330 core

out vec4 outColor;

uniform sampler2D colorTex;
uniform mat4 modelView;
uniform mat4 normal;

in vec2 vTexCoord;
in vec3 vnormal;

void main()
{
	if((gl_PrimitiveID % 2) == 1  ){
	outColor = normal * vec4(vnormal,0);
	}
	if((gl_PrimitiveID % 2) == 0){
	outColor = texture(colorTex, vTexCoord);
	}
}

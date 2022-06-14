#version 330 core

out vec4 outColor;

uniform sampler2D colorTex;
uniform mat4 modelView;
uniform mat4 normal;

in vec3 vnormal;

void main()
{
	//outColor = inverse(transpose(modelView)) * vec4(vnormal,0);
	outColor = normal * vec4(vnormal,0);
}

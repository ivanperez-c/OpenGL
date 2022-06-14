#version 330 core

out vec4 outColor;
uniform sampler2D colorTex;

in vec3 vnormal;

void main()
{
	outColor = abs(vec4(vnormal,0));
}

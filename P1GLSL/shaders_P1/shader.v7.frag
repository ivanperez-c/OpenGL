#version 330 core

out vec4 outColor;
uniform sampler2D colorTex;

in vec2 vTexCoord;

void main()
{
	//outColor = vec4(vTexCoord, vec2(1));
	outColor = texture(colorTex, vTexCoord);
}

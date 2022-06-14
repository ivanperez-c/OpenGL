#version 330 core

out vec4 outColor;

uniform sampler2D colorTex;
uniform mat4 modelView;
uniform mat4 normal;

in vec2 vTexCoord;
in vec3 vnormal;

void main()
{
	outColor = texture(colorTex, vTexCoord); 
	vec4 color = texture(colorTex, vTexCoord);
	 if (color.rgb == vec3(0.0,0.0,0.0))
      discard; 
}

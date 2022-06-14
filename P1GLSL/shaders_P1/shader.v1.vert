#version 330 core

in vec3 inPos;		


mat4 view;
mat4 proj;


void main()
{
	view = mat4(1.0);
	view[3].z = -3.0;
	//view[3] = vec4(vec2(0), -3.0, 1.0);
	//view[3].zw = vec2(-3.0, 1.0);

	float n = 1.0;
	float f = 5.0;
	proj = mat4(0.0);
	proj[0].x = 1.73205080757; // (1/tan(30 grados))
	proj[1].y = proj[0].x;
	proj[2].z = -(f + n)/(f - n);
	proj[2].w = -1.0;
	proj[3].z = -2.0 * n * f/(f - n);


	gl_Position =  proj * view * vec4(inPos, 1.0);
}

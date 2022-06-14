#version 330 core

in vec3 inPos;	
in vec3 inNormal;
in vec3 inColor;
uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 vcolor;

//Propiedades de las fuentes de luz
//uniform vec3 Ia;
vec3 Ia = vec3(0.1);
//uniform vec3 Il;
vec3 Il = vec3(1.0);
//uniform vec3 pl;
vec3 pl= vec3(0.0, 1.0, 0.0); //Este valor está en coordenadas de la cámara,
							//pero podría estar en coordenadas del mundo

//Propiedades que dependen del material
vec3 Ka;
vec3 Kd;
vec3 Ks;
float n;
vec3 N;
vec3 pp;

vec3 shade()
{
	vec3 c = vec3(0);

	//Ambiental
	c += Ia * Ka;

	//Difusa
	vec3 L = normalize(pl - pp);
	c += Il * Kd * dot(N, L);

	//Especular
	vec3 R = normalize(reflect(-L, N));
	//vec3 V = (cop - pp) = (vec3(0) - pp);;
	vec3 V = normalize(-pp);
	c += Il * Ks * pow(max(dot(R,V), 0), n);

	c = clamp(c, 0, 1);

	return c;
}

void main()
{
	//Ka = inColor;
	Ka = vec3(1, 0, 0);
	Kd = Ka;
	Ks = vec3(1);
	n = 5;


	pp = (modelView * vec4(inPos, 1.0)).xyz;
	N = normalize((normal * vec4(inNormal, 0.0)).xyz);

	vcolor = shade();

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

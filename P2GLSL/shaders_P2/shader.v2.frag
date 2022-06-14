#version 330 core

out vec4 outColor;

in vec3 vpos;
in vec3 vnormal;
in vec3 vcolor;

//Propiedades de las fuentes de luz
//uniform vec3 Ia;
vec3 Ia = vec3(0.1);
//uniform vec3 Il;
vec3 Il = vec3(1.0);
//uniform vec3 pl;
vec3 pl= vec3(0.0, 1.0, 0.0); //Este valor est� en coordenadas de la c�mara,
							//pero podr�a estar en coordenadas del mundo

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
	//Ka = vec3(1, 0, 0);
	Ka = vcolor;
	Kd = Ka;
	Ks = vec3(1);
	n = 30;

	pp = vpos;
	N = normalize(vnormal);

	outColor = vec4(shade(), 1.0);   
}

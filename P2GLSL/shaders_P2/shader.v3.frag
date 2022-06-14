#version 330 core

out vec4 outColor;

in vec3 vpos;
in vec3 vnormal;
in vec3 vcolor;
in vec2 vtexCoord;

//Propiedades de las fuentes de luz
//uniform vec3 Ia;
vec3 Ia = vec3(0.1);
//uniform vec3 Il;
vec3 Il = vec3(1.0);
//uniform vec3 pl;
vec3 pl= vec3(0.0, 1.0, 0.0); //Este valor está en coordenadas de la cámara,
							//pero podría estar en coordenadas del mundo



uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;


//Propiedades que dependen del material
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
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
	//vec3 V = (cop - pp) = (vec3(0) - pp);;
	vec3 V = normalize(-pp);
	vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(L+V);
	//c += Il * Ks * pow(max(dot(R,V), 0), n);
	c += Il * Ks * pow(max(dot(N, H), 0), n);
	
	c += Ke;

	c = clamp(c, 0, 1);

	return c;
}



void main()
{
	//Ka = vec3(1, 0, 0);
	Ka = texture(colorTex, vtexCoord).rgb;
	Kd = Ka;
	Ks = texture(specularTex, vtexCoord).rgb;
	Ke = texture(emiTex, vtexCoord).rgb;

	n = 200;

	pp = vpos;
	N = normalize(vnormal);

	outColor = vec4(shade(), 1.0);   
}

#version 330 core

out vec4 outColor;

in vec3 vpos;
in vec3 vnormal;
in vec3 vcolor;
in vec2 vtexCoord;
in vec3 vtangent;

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

//////
uniform sampler2D normalTex;
uniform sampler2D auxiliarTex;
/////

//Propiedades que dependen del material
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float n;
vec3 N;
vec3 pp;

const float C0 = 1.0; //constant attenuation
const float C1 = 0.2; //linear attenuation
const float C2 = 0.0; //quadratic attenuation

//Luz puntual 1
vec3 shade()
{
	vec3 c = vec3(0);
	vec3 bt = cross(vnormal, vtangent);

	//Ambiental
	c += Ia * Ka;

	//Difusa
	vec3 L = normalize(pl - pp);
	//L = vec3(dot(vtangent, L), dot(bt, L), dot(vnormal, L)); ///

	c += Il * Kd * dot(N, L);

	//Especular
	vec3 V = normalize(-pp);
	//V = vec3(dot(vtangent,L), dot(bt,L), dot(vnormal,L)); ///

	vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(L+V);
	c += Il * Ks * pow(max(dot(N, H), 0), n);
	
	c += Ke;

	c = clamp(c, 0, 1);

	return c;
}


void main()
{
	Ka = texture(colorTex, vtexCoord).rgb;
	Kd = Ka;
	Ks = texture(specularTex, vtexCoord).rgb;
	Ke = texture(emiTex, vtexCoord).rgb;

	n = 200;

	pp = vpos;
	N = normalize(vnormal);

	//Ka += texture(normalTex, vtexCoord).xyz;
	//Kd += texture(normalTex, vtexCoord).xyz;
	//Ks += texture(normalTex, vtexCoord).xyz;
	//Ke += texture(normalTex, vtexCoord).xyz;

	/*vec3 bt = vec3(normalize(cross(vnormal, inTangent)));
	
	mat4 normal = mat4(0.0f);
	normal[0].x = inTangent.x;
	normal[1].x = inTangent.y;
	normal[2].x = inTangent.z;

	normal[0].y = bt.x;
	normal[1].y = bt.y;
	normal[2].y = bt.z;

	normal[0].z = vnormal.x;
	normal[1].z = vnormal.y;
	normal[2].z = vnormal.z;

	normal[3].w = 1;

	vec4 paco = (normal * vec4(vpos, 1));
	vec2 pacoporras;
	//pacoporras.x = paco.x;
	pacoporras.x = paco.y;
	pacoporras.y = paco.z;*/

	//Luz puntual 1
	outColor = vec4(shade(), 1.0);   
	
	
}
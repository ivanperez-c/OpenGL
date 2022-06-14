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

//Luz puntual con atenuación de la distancia
vec3 shadeNi()
{
	vec3 L1 = pl - pp;
	vec3 L = normalize(L1);

    vec3 N = normalize(vnormal);
    vec3 V = normalize(vpos);
	vec3 H = normalize(L+V);

	float d = length(L1);
	float d0 = 1;
	float epsilon = 1;

	//Ambiental
	vec3 ambiental = Ia * Ka;
	
	//Difusa
	vec3 difusa = Il * Kd * dot(N, L);

	//Especular
	vec3 specular = Il * Ks * pow(max(dot(N, H), 0), n);
	
	// Luz emisiva
	vec3 emisiva = Ke;
	
	//fórmula CryEngine y Frostbite atenuación con la distancia
	float Fdiff = pow(1/(max(d,-10)),2); 

	//función ventana
	float Fwin = pow(max(1-pow(d/10,4),0),2);

	float Fdw = Fdiff*Fwin;
	ambiental  *= Fdw;
	difusa  *= Fdw;
	specular *= Fdw;
	emisiva *= Fdw;

	vec3 c = ambiental + difusa + specular + emisiva;
	
	c = clamp(c, 0, 1);

    //Parámetros de la niebla
	vec3 cf = c;
	vec3 cs = vec3 (0.1,0.1,0.1); 
	float zp = 0.1; 
	
	//Función f
	float op1 = dot(d,zp);
	float op2 = pow(op1,2);
	float f = pow(2.71828182,-op2);
	
	//función cp
	vec3 cp = vec3(0);
	cp += f*cs;
	cp += (1-f)*cf;

	c = cp+c;

	//Añadir niebla 
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

	//Luz puntual con atenuacion de la distancia
	outColor = vec4(shadeNi(), 1.0);

}

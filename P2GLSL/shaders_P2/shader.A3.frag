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

uniform mat4 view;

//Propiedades que dependen del material
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float n;
vec3 N;
vec3 pp;

//Luz direccional
vec3 shadeDi()
{
	vec3 c = vec3(0);

	//Ambiental
	c += Ia * Ka;
	
	//Difusa
	vec3 L = vec3(-1,1,0);
	c += Il * Kd * dot(N, L);

	//Especular
	vec3 V = normalize(-pp);
	vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(L+V);

	c += Il * Ks * pow(max(dot(N, H), 0), n);
	
	// Luz emisiva
	c += Ke;

	c = clamp(c, 0, 1);

	return c;
}

//Luz focal
vec3 shadeFo()
{
    float cutoff = 0.2;   // ángulo de apertura
    float intensity = 20; // intensidad del foco

    vec3 Lpos = (view * vec4(pl, 1)).xyz;
    vec3 L1 = pl - pp;
    float d = length(L1);

    vec3 N = normalize(vnormal);
    vec3 L = normalize(L1);
    vec3 V = normalize(vpos);

    vec3 D = vec3(0,0,-1);
    float spotEffect = dot(-L, D);

	if(spotEffect > cutoff) {
        spotEffect = pow(spotEffect, intensity);

    vec3 R = normalize(reflect(-L, N));

	//Ambiental
	vec3 ambiental = Ia * Ka;
    
	//Difusa
	vec3 difusa = Il * Kd * max(0.0, dot(N, L));
  
	//Especular
	vec3 specular = Il * Ks * pow(max(0.0, dot(R, V)), n);

	// Luz emisiva
	vec3 emisiva = Ke;

    //Atenuacion de la luz con formula Unreal
    float Fdiff = pow(1/(max(d,-10)),2);

	//función ventana
	float ventana = pow(max(1-pow(d/10,4),0),2);

	float va = Fdiff*ventana;
	ambiental  *= va;
	difusa  *= va;
	specular *= va;
	emisiva *= va;

    vec3 c = ambiental  + difusa + specular + emisiva;
    c = clamp(c, 0, 1);
    return c;

    }
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

	//Luz direccional
	//outColor = vec4(shadeDi(), 1.0);
	
	//Luz focal 
	//outColor = vec4(shadeFo(), 1.0);

	//Luz direccional y focal
	outColor = vec4(shadeDi(), 1.0) + vec4(shadeFo(), 1.0);

}

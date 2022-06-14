#include "BOX.h"
#include "auxiliar.h"


#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);

glm::mat4	model2 = glm::mat4(1.0f); 
glm::mat4	model3 = glm::mat4(1.0f); 
glm::mat4	model4 = glm::mat4(1.0f); 


glm::vec3	lightintensity = glm::vec3(1.0f); //Intensidad de la luz
glm::vec3	lightpos = glm::vec3(0.0, 0.0, 8.0); //Posición de la luz


//Tangentes y normales
float arrayTangentes[72]; //72 --> Tamaño del array de cubeVertexTangent
float arrayNormales[72]; //72 --> Tamaño del array de cubeVertexNormal

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir

unsigned int vshader; 
unsigned int fshader;
unsigned int program; 

unsigned int vshader2; 
unsigned int fshader2; 
unsigned int program2; 

//Variables 
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;
int uIncLightIntensity;
int uMyLightPos;
int uColorTex;
int uEmiTex;
int uSpecTex; 
int uNormalTex; 
int uView; 
int uModelViewMat2; 
int uModelViewProjMat2; 
int uNormalMat2; 
int uColorTex2; 
int uEmiTex2; 

//Atributos 
int inPos; 
int inColor;
int inNormal;
int inTexCoord;
int inTangent; 
int inPos2; 
int inColor2; 
int inNormal2;
int inTexCoord2; 

//Texturas
unsigned int colorTexId;
unsigned int emiTexId;
unsigned int specTexId;
unsigned int normalTexId; 
unsigned int tex2; 
unsigned int colorTexId2; 
unsigned int emiTexId2; 

//VAO
unsigned int vao;
unsigned int vao2; 
unsigned int vao3; 
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int tangentVBO; 
unsigned int triangleIndexVBO;

//Parámetros de la curva de bezier
float t = 0;//tiempo para la curva de bezier
float c = 0.005;

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB 
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initShader2(const char *vname, const char *fname); 
void initObj();
void destroy();

//Funciones para calcular tangentes y normales  
void generartangentes(glm::vec3 v0, glm::vec3 v1, glm::vec2 st1, glm::vec2 st2, int i);
void sacartangentesynormales(); 
void generarnormales(glm::vec3 v0, glm::vec3 v1, int i, int k);  

//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);


int main(int argc, char** argv) //perfecta
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	initContext(argc, argv);
	initOGL();

	initShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	initShader2("../shaders_P3/shader.v0.vert", "../shaders_P3/shader.v0.frag"); 
	initObj();
	
	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE); 
									
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas OGL");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
																		
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);

}
void initOGL() {  
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -15;

}
void destroy() {  

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);  
	glDeleteShader(vshader);
	glDeleteShader(fshader);         
	glDeleteProgram(program);       
	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &normalVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glDeleteVertexArrays(1, &vao);
}

void initShader(const char *vname, const char *fname) { 

	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");               
	uModelViewMat = glGetUniformLocation(program, "modelView");         
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj"); 
	
	//Variables de la luz  
	uMyLightPos = glGetUniformLocation(program, "myLightPos");               
	uIncLightIntensity = glGetUniformLocation(program, "incLightIntensity");   
	uView = glGetUniformLocation(program, "View");                          

	//Variables de las texturas
	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecTex = glGetUniformLocation(program, "specTex");    
	uNormalTex = glGetUniformLocation(program, "normalTex"); 

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
	inTangent = glGetAttribLocation(program, "inTangent");
}


void initShader2(const char *vname, const char *fname) {  

	vshader2 = loadShader(vname, GL_VERTEX_SHADER);
	fshader2 = loadShader(fname, GL_FRAGMENT_SHADER);

	program2 = glCreateProgram();
	glAttachShader(program2, vshader2);
	glAttachShader(program2, fshader2);
	glLinkProgram(program2);
	
	int linked;      
	glGetProgramiv(program2, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program2, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	uNormalMat2 = glGetUniformLocation(program2, "normal");
	uModelViewMat2 = glGetUniformLocation(program2, "modelView");
	uModelViewProjMat2 = glGetUniformLocation(program2, "modelViewProj");

	uColorTex2 = glGetUniformLocation(program2, "colorTex");
	uEmiTex2 = glGetUniformLocation(program2, "emiTex");

	inPos2 = glGetAttribLocation(program2, "inPos");
	inColor2 = glGetAttribLocation(program2, "inColor");
	inNormal2 = glGetAttribLocation(program2, "inNormal");
	inTexCoord2 = glGetAttribLocation(program2, "inTexCoord");
}

void initObj() { 

	//Objeto 1 - Cubo central
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Calculamos tangentes y normales
	sacartangentesynormales();

	if (inPos != -1) {
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1) {
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, arrayNormales, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1) {
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1) {
		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexTangent, GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);

	model = glm::mat4(1.0f);

	colorTexId = loadTex("../img/color2.png"); 
	emiTexId = loadTex("../img/emissive.png"); 
	specTexId = loadTex("../img/specMap.png"); 
	normalTexId = loadTex("../img/normal.png");
	tex2 = loadTex("../img/normal.png");


	//cubo sin texturas
	glGenVertexArrays(2, &vao2);
	glBindVertexArray(vao2);

	if (inPos != -1) {
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1) {
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);    
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, arrayNormales, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1) {
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	glGenBuffers(2, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);


	//Objeto3
	glGenVertexArrays(3, &vao3);
	glBindVertexArray(vao3);

	if (inPos2 != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos2);
	}
	if (inColor2 != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor2);
	}
	if (inNormal2 != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal2);
	}
	if (inTexCoord2 != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord2);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);

	model4 = glm::mat4(1.0f);

	colorTexId2 = loadTex("../img/color2.png");
	emiTexId2 = loadTex("../img/emissive.png");
}


GLuint loadShader(const char *fileName, GLenum type) {
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;

	GLint compiled;    
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error in file " << fileName << ":" << std::endl
			<< logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	return shader;
}

unsigned int loadTex(const char *fileName) {
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map) {
		std::cout << "Error cargando el fichero: " << fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);
	delete[] map;

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	//Filtro Anisotrópico
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	return texId;
}

void renderFunc() {  
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program); 

	//////////////////////////////////////////////////////////////////////////
	//Inicio renderizado cubo 1 - Cubo principal
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1) glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	if (uModelViewProjMat != -1) glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	if (uNormalMat != -1) glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));
	if (uView != -1) glUniformMatrix4fv(uView, 1, GL_FALSE, &(view[0][0]));
	if (uIncLightIntensity != -1) glUniform3fv(uIncLightIntensity, 1, &(lightintensity.x));
	if (uMyLightPos != -1) glUniform3fv(uMyLightPos, 1, &(lightpos.x));

	//Texturas
	if (uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}
	if (uSpecTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, specTexId);
		glUniform1i(uSpecTex, 2);
	}
	if (normalTexId != -1) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 3);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
	//Fin renderizado cubo 1 - cubo principal
	//////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////
	//Inicio renderizado cubo 2 - Cubo abajo a la izquierda
	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::translate(model, glm::vec3(-2.0f));

	glm::mat4 modelView2 = view * model2;
	glm::mat4 modelViewProj2 = proj * view * model2;
	glm::mat4 normal2 = glm::transpose(glm::inverse(modelView2));

	if (uModelViewMat != -1) glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView2[0][0]));
	if (uModelViewProjMat != -1) glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj2[0][0]));
	if (uNormalMat != -1) glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal2[0][0]));

	//Texturas
	if (uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}
	if (uSpecTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, specTexId);
		glUniform1i(uSpecTex, 2);
	}
	if (normalTexId != -1) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 3);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
	//Fin renderizado cubo 2 - Cubo abajo a la izquierda
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	//Inicio renderizado cubo 4 - Cubo de la curva de bezier
	//Parámetros para definir la curva de bezier
	glm::mat4 model4 = glm::mat4(1.0f);

	glm::mat3 pcontrol = glm::transpose(glm::mat3(glm::vec3(-8.0, 0.0, 2.0), glm::vec3(0.0, -12.0, 0.0), glm::vec3(8.0, 0.0, -2.0)));//Matriz de puntos de control
	glm::mat3 curva = glm::transpose(glm::mat3(glm::vec3(1.0, 0.0, 0.0), glm::vec3(-2.0, 2.0, 0.0), glm::vec3(1.0, -2.0, 1.0)));//Matriz de la curva
	glm::vec3 tiempo = glm::vec3(1.0, t, t*t);//Matriz del tiempo
	glm::vec3 resultado = tiempo * curva * pcontrol; //puntos x , y ,z en cada momento 
											
	model4 = glm::translate(model4, glm::vec3(model[3].x, model[3].y, model[3].z));
	model4 = glm::translate(model4, resultado);
	
	t = t + c;
	if (t >= 1)
		c = -0.005;
	if (t <= 0)
		c = 0.005;

	glm::mat4 modelView4 = view * model4;
	glm::mat4 modelViewProj4 = proj * view * model4;
	glm::mat4 normal4 = glm::transpose(glm::inverse(modelView4));

	if (uModelViewMat != -1) glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView4[0][0]));
	if (uModelViewProjMat != -1) glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj4[0][0]));
	if (uNormalMat != -1) glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal4[0][0]));

	//Texturas
	if (uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}
	if (uSpecTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, specTexId);
		glUniform1i(uSpecTex, 2);
	}
	if (normalTexId != -1) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 3);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
	//Fin renderizado cubo 4 - Cubo de la curva de bezier
	//////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////
	//Inicio renderizado cubo 3 - Cubo de colores - utiliza otro programa
	glUseProgram(program2);
	model3 = glm::translate(model, glm::vec3(2.5f));

	glm::mat4 modelView3 = view * model3;
	glm::mat4 modelViewProj3 = proj * view * model3;
	glm::mat4 normal3 = glm::transpose(glm::inverse(modelView3));

	if (uModelViewMat2 != -1) glUniformMatrix4fv(uModelViewMat2, 1, GL_FALSE, &(modelView3[0][0]));
	if (uModelViewProjMat2 != -1) glUniformMatrix4fv(uModelViewProjMat2, 1, GL_FALSE, &(modelViewProj3[0][0]));
	if (uNormalMat2 != -1) glUniformMatrix4fv(uNormalMat2, 1, GL_FALSE, &(normal3[0][0]));

	if (uColorTex2 != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	glBindVertexArray(vao3);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
	//Fin renderizado cubo 3 - Cubo de colores - utiliza otro programa
	//////////////////////////////////////////////////////////////////////////

	glUseProgram(NULL);
	glutSwapBuffers();
}


void resizeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	float aspectratio = (float(width) / float(height));
	proj[0].x = 1.0 / (aspectratio * (tan(3.1415 / 6)));
	proj[1].y = 1.0 / (tan(3.1415 / 6));
	glutPostRedisplay();

}

void idleFunc() {
	
	model = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
	glm::mat4 viewAux = glm::mat4(1.0f);

	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
	switch (key) {
	case 'w':
		viewAux[3].z = viewAux[3].z + 2.0f;
		break;
	case 'a':
		viewAux[3].x = viewAux[3].x + 2.0f;
		break;
	case 's':
		viewAux[3].z = viewAux[3].z - 2.0f;
		break;
	case 'd':
		viewAux[3].x = viewAux[3].x - 2.0f;
		break;
	case 'q':
		viewAux = glm::rotate(viewAux, 0.1f, glm::vec3(0.0, -1.0, 0.0));
		break;
	case 'e':
		viewAux = glm::rotate(viewAux, 0.1f, glm::vec3(0.0, 1.0, 0.0));
		break;
	case 'f':
		//Mueve la luz hacia la izquierda
		lightpos = lightpos + glm::vec3(-1.0, 0, 0);
		break;
	case 'g':
		//Mueve la luz hacia abajo
		lightpos = lightpos + glm::vec3(0, -1.0, 0);
		break;
	case 'h':
		//Mueve la luz hacia la derecha
		lightpos = lightpos + glm::vec3(1.0, 0, 0);
		break;
	case 't':
		//Mueve la luz hacia arriba
		lightpos = lightpos + glm::vec3(0, 1.0, 0);
		break;
	case 'j':
		//Mueve la luz hacia atras
		lightpos = lightpos + glm::vec3(0, 0, 1.0);
		break;
	case 'u':
		//Mueve la luz hacia delante
		lightpos = lightpos + glm::vec3(0, 0, -1.0);
		break;
	case 'i':
		//Aumenta la intensidad de la luz
		if (lightintensity.x < 1)lightintensity += glm::vec3(0.1, 0.1, 0.1);
		break;
	case 'k':
		//Disminuye la intensidad de la luz
		if (lightintensity.x > 0)lightintensity -= glm::vec3(0.1, 0.1, 0.1);
		break;
	case 27:
		exit(0);
		break;
	}

	view = viewAux * view;
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	glm::mat4 rotation = glm::mat4(1.0);

	if (state == 0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";

	if (button == 0) {
		std::cout << "de la izquierda del ratón " << std::endl;
		rotation = glm::rotate(rotation, 0.1f, glm::vec3(0.0, -1.0, 0.0));
		view = rotation * view;
	}
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl; {
		rotation = glm::rotate(rotation, 0.1f, glm::vec3(0.0, 1.0, 0.0));
		view = rotation * view;
	}
	glutPostRedisplay();
	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

//Calculamos las tangentes y las normales de los cubos
void sacartangentesynormales() {
	int k = 0;
	for (int i = 0; i < 36; i += 3) {
		glm::vec3 P0 = glm::vec3(cubeVertexPos[cubeTriangleIndex[i] * 3], cubeVertexPos[cubeTriangleIndex[i] * 3 + 1], cubeVertexPos[cubeTriangleIndex[i] * 3 + 2]);
		glm::vec3 P1 = glm::vec3(cubeVertexPos[cubeTriangleIndex[i + 1] * 3], cubeVertexPos[cubeTriangleIndex[i + 1] * 3 + 1], cubeVertexPos[cubeTriangleIndex[i + 1] * 3 + 2]);
		glm::vec3 P2 = glm::vec3(cubeVertexPos[cubeTriangleIndex[i + 2] * 3], cubeVertexPos[cubeTriangleIndex[i + 2] * 3 + 1], cubeVertexPos[cubeTriangleIndex[i + 2] * 3 + 2]);

		glm::vec2 uv0 = glm::vec2(cubeVertexTexCoord[cubeTriangleIndex[i] * 2], cubeVertexTexCoord[cubeTriangleIndex[i] * 2 + 1]);
		glm::vec2 uv1 = glm::vec2(cubeVertexTexCoord[cubeTriangleIndex[i + 1] * 2], cubeVertexTexCoord[cubeTriangleIndex[i + 1] * 2 + 1]);
		glm::vec2 uv2 = glm::vec2(cubeVertexTexCoord[cubeTriangleIndex[i + 2] * 2], cubeVertexTexCoord[cubeTriangleIndex[i + 2] * 2 + 1]);

		glm::vec3 V0 = glm::vec3(P1.x - P0.x, P1.y - P0.y, P1.z - P0.z);
		glm::vec3 V1 = glm::vec3(P2.x - P0.x, P2.y - P0.y, P2.z - P0.z);
		glm::vec2 st1 = glm::vec2(uv1.x - uv0.x, uv1.y - uv0.y);
		glm::vec2 st2 = glm::vec2(uv2.x - uv0.x, uv2.y - uv0.y);

		 generartangentes(V0, V1, st1, st2, i);
		if (i % 2 == 0) {
			generarnormales(V0, V1, i, k);
			k = k + 12;
		}
	}
}

//calculamos las tangetes de los cubos
void  generartangentes(glm::vec3 v0, glm::vec3 v1, glm::vec2 st1, glm::vec2 st2, int i) {
	float coef = 1 / (st1.x*st2.y - st1.y * st2.x);
	arrayTangentes[cubeTriangleIndex[i] * 3] += coef*(v0.x*st2.y - v1.x*st1.y);
	arrayTangentes[cubeTriangleIndex[i] * 3 + 1] += coef*(v0.y*st2.y - v1.y*st1.y);
	arrayTangentes[cubeTriangleIndex[i] * 3 + 2] += coef*(v0.z*st2.y - v1.z*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 1] * 3] += coef*(v0.x*st2.y - v1.x*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 1] * 3 + 1] += coef*(v0.y*st2.y - v1.y*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 1] * 3 + 2] += coef*(v0.z*st2.y - v1.z*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 2] * 3] += coef*(v0.x*st2.y - v1.x*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 2] * 3 + 1] += coef*(v0.y*st2.y - v1.y*st1.y);
	arrayTangentes[cubeTriangleIndex[i + 2] * 3 + 2] += coef*(v0.z*st2.y - v1.z*st1.y);
}

//calculamos las normales de los cubos
void generarnormales(glm::vec3 v0, glm::vec3 v1, int i, int k) {
	glm::vec3 aux;
	aux = glm::normalize(glm::cross(v0, v1));
	for (int j = i; j < i + 4; j++) {
		arrayNormales[k] = aux.x;
		arrayNormales[k + 1] = aux.y;
		arrayNormales[k + 2] = aux.z;
		k = k + 3;
	}
}







#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;
int objId2 = -1;
int objId3 = -1; 

//Parámetros de la curva de bezier 
float t = 0.1; //tiempo 
float v = 0.005; //velocidad del cubo

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

glm::mat4 view = glm::mat4(1.0); 
glm::mat4 proj = glm::mat4(0.0); 
int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));
	if (!IGlib::init("../shaders_P1/shader.A5.vert", "../shaders_P1/shader.A5.frag"))
		return -1;
   
	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	view[3].z = -6.0; // al valor z de la cuarta columna le asignamos el -6 (valor de la cámara)

	//matriz de proyección
	float n = 1.0;
	float f = 15.0;
	proj[0].x = 1.73205080757; //(1/tan(30 grados))
	proj[1].y = proj[0].x;
	proj[2].z = -(f + n) / (f - n);
	proj[2].w = -1.0;
	proj[3].z = (-2.0 * n * f) / (f - n);

	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);

	objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos2, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	objId3 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	IGlib::setModelMat(objId2, modelMat);
	IGlib::setModelMat(objId3, modelMat);

	//Incluir texturas aquí.
	IGlib::addColorTex(objId, "../img/negro.png");
	IGlib::addColorTex(objId2, "../img/color.png");
	IGlib::addColorTex(objId3, "../img/azul.png");


  //CBs
  IGlib::setResizeCB(resizeFunc);
  IGlib::setIdleCB(idleFunc);
  IGlib::setKeyboardCB(keyboardFunc);
  IGlib::setMouseCB(mouseFunc);
  IGlib::setMouseMoveCB(mouseMotionFunc);

	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
	//Ajusta el aspect ratio al tamaño de la venta
	glm::mat4 proj2 = glm::mat4(0.0f); //Inicialización de la matriz 

	//Iniciar variables de la matriz
	float a_ratio = static_cast<float>(width) / height; //Nos aseguramos que la a_ratio sea un float y no se convierta en int
	float n = 1.0;
	float f = 15.0;
	float right = a_ratio;
	float left = -a_ratio;
	float top = 1.0f;
	float bottom = -1.0f;

	//Creación de la matriz de normalización perspectiva de OpenGL
	//Primera fila
	proj2[0].x = (2 * n) / (right - left); 
	proj2[0].z = (right + left) / (right - left);
	//Segunda fila
	proj2[1].y = (2 * n) / (top - bottom); 
	proj2[1].z = (top + bottom) / (top - bottom);
	//Tercera fila
	proj2[2].z = (-(f + n)) / (f - n);
	proj2[2].w = -2 * (f * n) / (f - n);
	//Cuarta fila
	proj2[3].z = -1;

	IGlib::setProjMat(proj2);
}

void idleFunc()
{
	static float angle = 0.0f;
	angle = (angle < 2 * 3.14159f) ? (angle + 0.01f) : 0.0f;

	//Cubo 1
	glm::mat4 model = glm::rotate(glm::mat4(1.0), angle, glm::vec3(1.0f, 1.0f, 0.0f));
	IGlib::setModelMat(objId, model);

	//Cubo 2
	glm::mat4 model2 = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	IGlib::setModelMat(objId2, model2);

	// Objeto 3 (Curva de Bezier)
	glm::mat3 puntosControl = glm::transpose(glm::mat3(glm::vec3(-9.0, 0.0, 2.0), glm::vec3(0.0, 11.0, 0.0), glm::vec3(9.0, 0.0, -2.0))); 
	glm::mat3 curvaB = glm::transpose(glm::mat3(glm::vec3(1.0, 0.0, 0.0), glm::vec3(-2.0, 2.0, 0.0), glm::vec3(1.0, -2.0, 1.0)));
	glm::vec3 tiempo = glm::vec3(1.0, t, t * t);
	glm::vec3 Bezier = tiempo * curvaB * puntosControl; 
		

	t = t + v; //Movimiento con el tiempo
	if (t >= 1)
		v = -0.005;
	if (t <= 0)
		v = 0.005;

	glm::mat4 model3 = glm::mat4(1.0f);
	model3 = glm::translate(model3, glm::vec3(model[3].x, model[3].y, model[3].z));
	model3 = glm::translate(model3, Bezier);
	
	IGlib::setModelMat(objId3, model3);
}


//Variables globales para el control de la cámara
static glm::vec3 Pos = glm::vec3(0, 0, 6);
static glm::vec3 UP = glm::vec3(0, 1, 0);
static glm::vec3 LookAt = glm::vec3(0.0, 0.0, -1.0);

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

	glm::vec3 Right = cross(LookAt, UP);
	float inc = 0.2f;

	//static float rotate = -1.57079632679; // PI/2
	static glm::vec2 rotate = glm::vec2(0, -1.57079632679);
	//float cosR = cos(rotate);
	glm::vec2 cosR = cos(rotate);
	//float sinR = sin(rotate);
	glm::vec2 sinR = sin(rotate);
	//LookAt = glm::vec3(cosR, 0, sinR);
	glm::vec3 LookAt = glm::vec3(cosR.y * cosR.x, sinR.x, sinR.y * cosR.x);

	//Control del movimiento de la cámara
	switch (key) {
	case 'w':
		Pos += LookAt * inc;
		break;
	case 's':
		Pos -= LookAt * inc;
		break;
	case 'a':
		Pos += Right * inc;
		break;
	case 'd':
		Pos -= Right * inc;
		break;
	case 'q':
		rotate.y -= inc;
		break;
	case 'e':
		rotate.y += inc;
		break;
	case 'r':
		rotate.x += inc;
		break;
	case 'f':
		rotate.x -= inc;
		break;
	};

	//Definimos los vectores de la matriz de vista
	glm::vec3 k = normalize(-LookAt);
	glm::vec3 j = normalize(UP - dot(k, UP) * k);
	glm::vec3 i = cross(j, k);

	//Creamos la matriz vista
	glm::mat4 view2 = glm::mat4(i.x, i.y, i.z, 0,
		j.x, j.y, j.z, 0,
		k.x, k.y, k.z, 0,
		Pos.x, Pos.y, Pos.z, 1);
	IGlib::setViewMat(inverse(view2));
}

void mouseFunc(int button, int state, int x, int y)
{
	glm::vec3 centre = glm::vec3(0.0, 0.0, 0.0);
	if (state == 0) {
		std::cout << "Se ha pulsado el botón ";
	}
	else {
		std::cout << "Se ha soltado el botón ";
	}

	if (button == 0) {
		std::cout << "de la izquierda del ratón " << std::endl;
		if (Pos.z <= 0 && Pos.x <= 0) {
			Pos -= glm::vec3(-0.5, 0.0, 0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z >= 0 && Pos.x <= 0) {
			Pos -= glm::vec3(0.5, 0.0, 0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z >= 0 && Pos.x >= 0) {
			Pos -= glm::vec3(0.5, 0.0, -0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z <= 0 && Pos.x >= 0) {
			Pos -= glm::vec3(-0.5, 0.0, -0.5);
			LookAt = centre - Pos;
		}
	}
	if (button == 1) {

	}
	if (button == 2) {
		std::cout << "de la derecha del ratón " << std::endl;
		std::cout << "central del ratón " << std::endl;
		if (Pos.z <= 0 && Pos.x <= 0) {
			Pos += glm::vec3(-0.5, 0.0, 0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z >= 0 && Pos.x <= 0) {
			Pos += glm::vec3(0.5, 0.0, 0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z >= 0 && Pos.x >= 0) {
			Pos += glm::vec3(0.5, 0.0, -0.5);
			LookAt = centre - Pos;
		}
		if (Pos.z <= 0 && Pos.x >= 0) {
			Pos += glm::vec3(-0.5, 0.0, -0.5);
			LookAt = centre - Pos;
		}
	}

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;


	//Definimos los vectores de la matriz de vista
	glm::vec3 k = normalize(-LookAt);
	glm::vec3 j = normalize(UP - dot(k, UP) * k);
	glm::vec3 i = cross(j, k);

	//Creamos la matriz vista
	glm::mat4 view3 = glm::mat4(i.x, i.y, i.z, 0,
		j.x, j.y, j.z, 0,
		k.x, k.y, k.z, 0,
		Pos.x, Pos.y, Pos.z, 1);
	IGlib::setViewMat(inverse(view3));
}

void mouseMotionFunc(int x, int y)
{
}

#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P2/shader.O2.vert", "../shaders_P2/shader.O2.frag"))
		return -1;
  //Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	glm::mat4 view = glm::mat4(1.0);
	view[3].z = -6;

	glm::mat4 proj = glm::mat4(1.0);
	float f = 1.0f / tan(3.141592f / 6.0f);
	float far = 10.0f;
	float near = 0.1f;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);

	IGlib::addColorTex(objId, "../img/color.png");
	IGlib::addColorTex(objId, "../img/color.png");
	IGlib::addEmissiveTex(objId, "../img/emissive.png");
	IGlib::addNormalTex(objId, "../img/normal.png");
	IGlib::addSpecularTex(objId, "../img/specMap.png");
	// IGlib::addAuxiliarTex(objId, "../img/color2.png");

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	
	//CBs
	IGlib::setIdleCB(idleFunc);
	IGlib::setResizeCB(resizeFunc);
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
	glm::mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	
	modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	IGlib::setModelMat(objId, modelMat);
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

	static float rotate = -1.57079632679; // PI/2
	float cosR = cos(rotate);
	float sinR = sin(rotate);
	LookAt = glm::vec3(cosR, 0, sinR);

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
		rotate -= 0.05;
		break;
	case 'e':
		rotate += 0.05;
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
	if (state==0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y)
{

}

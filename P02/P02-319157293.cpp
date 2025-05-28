//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá
static const char* vShaderCuboRojo = "shaders/shaderCuboRojo.vert";
static const char* fShaderCuboRojo = "shaders/shaderCuboRojo.frag";
static const char* vShaderCuboCafe = "shaders/shaderCuboCafe.vert";
static const char* fShaderCuboCafe = "shaders/shaderCuboCafe.frag";
static const char* vShaderCuboVerde = "shaders/shaderCuboVerde.vert";
static const char* fShaderCuboVerde = "shaders/shaderCuboVerde.frag";
static const char* vShaderPiramideAzul = "shaders/shaderPiramideAzul.vert";
static const char* fShaderPiramideAzul = "shaders/shaderPiramideAzul.frag";
static const char* vShaderPiramideVerde = "shaders/shaderPiramideVerde.vert";
static const char* fShaderPiramideVerde = "shaders/shaderPiramideVerde.frag";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
			// Letra J
		   -0.7,0.5,0.0,						1.0f,	0.0f,	0.0f,
		   -0.4,0.5,0.0,						1.0f,	0.0f,	0.0f,
		   -0.4,0.4,0.0,						1.0f,	0.0f,	0.0f,
		   -0.7,0.5,0.0,						1.0f,	0.0f,	0.0f,
		   -0.4,0.4,0.0,						1.0f,	0.0f,	0.0f,
		   -0.7,0.4,0.0,						1.0f,	0.0f,	0.0f,
		   -0.6,0.4,0.0,						1.0f,	0.0f,	0.0f,
		   -0.5,0.5,0.0,						1.0f,	0.0f,	0.0f,
		   -0.6,0.1,0.0,						1.0f,	0.0f,	0.0f,
		   -0.5,0.5,0.0,						1.0f,	0.0f,	0.0f,
		   -0.6,0.1,0.0,						1.0f,	0.0f,	0.0f,
		   -0.5,0.1,0.0,						1.0f,	0.0f,	0.0f,
		   -0.7,0.1,0.0, 						1.0f,	0.0f,	0.0f,
		   -0.5,0.0,0.0,						1.0f,	0.0f,	0.0f,
		   -0.5,0.1,0.0,						1.0f,	0.0f,	0.0f,
		   -0.7,0.1,0.0,						1.0f,	0.0f,	0.0f,
		   -0.5,0.0,0.0,						1.0f,	0.0f,	0.0f,
		   -0.7,0.0,0.0,						1.0f,	0.0f,	0.0f,

		   // Letra B
		  -0.323,0.0,0.0,						0.0f,	1.0f,	0.0f,
		-0.323,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.0,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.323,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.0,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.501,0.0,						0.0f,	1.0f,	0.0f,
		-0.120,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.044,0.300,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.300,0.0,						0.0f,	1.0f,	0.0f,
		-0.120,0.449,0.0,						0.0f,	1.0f,	0.0f,
		-0.044,0.300,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.244,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.300,0.0,						0.0f,	1.0f,	0.0f,
		-0.044,0.300,0.0,						0.0f,	1.0f,	0.0f,
		-0.048,0.201,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.201,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.244,0.0,						0.0f,	1.0f,	0.0f,
		-0.117,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.047,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.048,0.201,0.0,						0.0f,	1.0f,	0.0f,
		-0.048,0.201,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.201,0.0,						0.0f,	1.0f,	0.0f,
		-0.117,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.0,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.047,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.046,0.0,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.053,0.0,						0.0f,	1.0f,	0.0f,
		-0.252,0.0,0.0,						0.0f,	1.0f,	0.0f,

		-0.253,0.335,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.335,0.0,						0.0f,	1.0f,	0.0f,
		-0.255,0.170,0.0,						0.0f,	1.0f,	0.0f,
		-0.118,0.170,0.0,						0.0f,	1.0f,	0.0f,
		-0.119,0.335,0.0,						0.0f,	1.0f,	0.0f,
		-0.258,0.170,0.0,						0.0f,	1.0f,	0.0f,

		// Letra G
		0.333,0.501,0.0,						0.0f,	0.0f,	1.0f,
		0.333,0.434,0.0,						0.0f,	0.0f,	1.0f,
		0.171,0.500,0.0,						0.0f,	0.0f,	1.0f,
		0.065,0.436,0.0,						0.0f,	0.0f,	1.0f,
		0.333,0.434,0.0,						0.0f,	0.0f,	1.0f,
		0.171,0.500,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.435,0.0,						0.0f,	0.0f,	1.0f,
		0.065,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.065,0.436,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.435,0.0,						0.0f,	0.0f,	1.0f,
		0.065,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.095,0.036,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.065,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.157,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.095,0.036,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.2,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.136,0.102,0.0,						0.0f,	0.0f,	1.0f,
		0.157,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.356,0.194,0.0,						0.0f,	0.0f,	1.0f,
		0.357,0.041,0.0,						0.0f,	0.0f,	1.0f,
		0.2,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.357,0.041,0.0,						0.0f,	0.0f,	1.0f,
		0.2,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.3,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.248,0.060,0.0,						0.0f,	0.0f,	1.0f,
		0.2,0.0,0.0,						0.0f,	0.0f,	1.0f,
		0.160,0.062,0.0,						0.0f,	0.0f,	1.0f,
		0.278,0.060,0.0,						0.0f,	0.0f,	1.0f,
		0.278,0.150,0.0,						0.0f,	0.0f,	1.0f,
		0.356,0.194,0.0,						0.0f,	0.0f,	1.0f,
		0.278,0.150,0.0,						0.0f,	0.0f,	1.0f,
		0.356,0.194,0.0,						0.0f,	0.0f,	1.0f,
		0.321,0.227,0.0,						0.0f,	0.0f,	1.0f,
		0.278,0.150,0.0,						0.0f,	0.0f,	1.0f,
		0.321,0.227,0.0,						0.0f,	0.0f,	1.0f,
		0.250,0.170,0.0,						0.0f,   0.0f,   1.0f,
		0.321,0.227,0.0,						0.0f,	0.0f,	1.0f,
		0.250,0.170,0.0,						0.0f,	0.0f,	1.0f,
		0.202,0.229,0.0,                     0.0f,   0.0f,   1.0f,
		0.250, 0.170, 0.0,					0.0f,   0.0f,   1.0f,
		0.202, 0.229, 0.0,					0.0f,   0.0f,   1.0f,
		0.200,0.169,0.0,						0.0f,   0.0f,   1.0f,
		0.136,0.382,0.0,						0.0f,   0.0f,   1.0f,
		0.202,0.435,0.0,						0.0f,   0.0f,   1.0f,
		0.136, 0.435, 0.0,					0.0f,   0.0f,   1.0f,

	};

	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 630);
	meshColorList.push_back(letras); // 0

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};


	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo); // 1

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde); // 2

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul); // 3


	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde); // 4

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo); // 5


	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe); // 6


}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); // 0

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2); // 1

	Shader* shader3 = new Shader(); //shader para usar índices: objetos: cubo rojo
	shader3->CreateFromFiles(vShaderCuboRojo, fShaderCuboRojo);
	shaderList.push_back(*shader3); // 2

	Shader* shader4 = new Shader(); //shader para usar índices: objetos: cubos verde
	shader4->CreateFromFiles(vShaderCuboVerde, fShaderCuboVerde);
	shaderList.push_back(*shader4); // 3

	Shader* shader5 = new Shader(); //shader para usar índices: objetos: Piramide Azul
	shader5->CreateFromFiles(vShaderPiramideAzul, fShaderPiramideAzul);
	shaderList.push_back(*shader5); // 4

	Shader* shader6 = new Shader(); //shader para usar índices: objetos: cubo cafe
	shader6->CreateFromFiles(vShaderCuboCafe, fShaderCuboCafe);
	shaderList.push_back(*shader6); // 5

	Shader* shader7 = new Shader(); //shader para usar índices: objetos: piramide verde ocuro
	shader7->CreateFromFiles(vShaderPiramideVerde, fShaderPiramideVerde);
	shaderList.push_back(*shader7); // 6

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);


		// Letras (Iniciales JBG)
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.25f));
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();


		//Para el cubo rojo se usa el set de shaders con índice 2 en ShaderList
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para el cubo verde se usa el set de shaders con índice 3 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		// Cubo verde (ventana derecha)
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::translate(model, glm::vec3(0.65f, -0.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde (ventana izquierda)
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::translate(model, glm::vec3(-0.65f, -0.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde (puerta)
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::translate(model, glm::vec3(0.0f, -2.4f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//Para la piramide azul se usa el set de shaders con índice 4 en ShaderList
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		// Piramide azul (techo)
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.2f, 0.8f, 0.75f));
		model = glm::translate(model, glm::vec3(0.242f, 0.75f, -4.0f));
		model = glm::rotate(model, 88 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Para el cubo cafe se usa el set de shaders con índice 5 en ShaderList
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		// Cubo cafe (tronco izquierdo)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.9f, -4.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo cafe (tronco derecho)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.9f, -4.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para la piramide verde osucuro se usa el set de shaders con índice 6 en ShaderList
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		// Piramide verde (arbol izquierdo) 
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.50f, 0.7f, 0.25f));
		model = glm::translate(model, glm::vec3(-1.5f, -0.63f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Piramide verde (arbol derecho) 
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.50f, 0.7f, 0.25f));
		model = glm::translate(model, glm::vec3(1.5f, -0.63f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}

	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
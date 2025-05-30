/*
Pr�ctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture DadoOcta;

// MI Practica6
Model Dado_M;
Model Car;
Model Car_Parabrisas;
Model Car_Chasis;
Model Car_Faros;
Model Car_Tire;
Model Car_Trunk;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.479f,	0.315f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.718f,	0.315f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.718f, 0.629f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.479f, 0.629f,		0.0f,	0.0f,	-1.0f,	//3

		// right
		0.5f, -0.5f,  0.5f,	    0.718f, 0.315f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.957f,	0.316f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.957f,	0.629f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.718f, 0.629f,		-1.0f,	0.0f,	0.0f,

		// back
		-0.5f, -0.5f, -0.5f,	0.718f, 0.629f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.479f,	0.629f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.479f,	0.946f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.719f,	0.946f,		0.0f,	0.0f,	1.0f,

		// left
		-0.5f, -0.5f,  -0.5f,	0.239f, 0.315f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.479f,	0.315f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.479f,	0.629f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.239f,	0.629f,		1.0f,	0.0f,	0.0f,

		// bottom
		-0.5f, -0.5f,  0.5f,	0.479f,	0.315f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.719f,	0.315f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.719f,	0.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.479f,	0.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 -0.5f, 0.5f,  0.5f,	0.0f,   0.315f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.239f,	0.315f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.239f,	0.629f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	0.629f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearDado8Caras()
{
	// V�rtices del dado
	GLfloat dado_vertices[] = {

		// x      y      z			S      T				NX      NY      NZ

		   // frontal arriba
		   0.0f,  0.5f,   0.0f,		0.390f, 1.000f,  0.0f,  -1.0f,  0.0f,  // Pico Alto
		  -0.5f, -0.0f,   0.5f,     0.197f, 0.668f,  0.0f,  -1.0f,  0.0f,
		   0.5f, -0.0f,   0.5f,     0.592f, 0.668f,  0.0f,  -1.0f,  0.0f,

		   // trasera arriba
		   0.0f,  0.5f,   0.0f,		0.390f, 1.000f,  0.0f,  -1.0f,  0.0f,  // Pico Alto
		  -0.5f, -0.0f,  -0.5f,     0.197f, 0.668f,  0.0f,  -1.0f,  0.0f,
		   0.5f, -0.0f,  -0.5f,     0.003f, 1.000f,  0.0f,  -1.0f,  0.0f,

		   // izquierda arriba
		   0.0f,  0.5f,   0.0f,		0.786f, 0.999f,  0.0f,  -1.0f,  0.0f,  // Pico Alto
		  -0.5f, -0.0f,  -0.5f,     0.390f, 1.000f,  0.0f,  -1.0f,  0.0f,
		  -0.5f, -0.0f,   0.5f,     0.592f, 0.668f,  0.0f,  -1.0f,  0.0f,

		  // derecha arriba
		  0.0f,  0.5f,   0.0f,		0.592f, 0.668f,  0.0f,  -1.0f,  0.0f,  // Pico Alto
		  0.5f, -0.0f,   0.5f,      0.197f, 0.668f,  0.0f,  -1.0f,  0.0f,
		  0.5f, -0.0f,  -0.5f,      0.399f, 0.336f,  0.0f,  -1.0f,  0.0f,

		  // frontal abajo
		  0.0f, -0.5f,   0.0f,		0.594f, 0.013f,  0.0f,  -1.0f,  0.0f,  // Pico Bajo
		 -0.5f, -0.0f,   0.5f,      0.399f, 0.336f,  0.0f,  -1.0f,  0.0f,
		  0.5f, -0.0f,   0.5f,      0.791f, 0.336f,  0.0f,  -1.0f,  0.0f,

		  // trasera abajo
		  0.0f, -0.5f,   0.0f,		0.592f, 0.668f,  -1.0f,  0.0f,  0.0f, // Pico Bajo
		 -0.5f, -0.0f,  -0.5f,      0.399f, 0.336f,  -1.0f,  0.0f,  0.0f,
		  0.5f, -0.0f,  -0.5f,      0.791f, 0.336f,  -1.0f,  0.0f,  0.0f,

		  // izquierda abajo
		  0.0f, -0.5f,   0.0f,		0.791f, 0.336f,  0.0f,  -1.0f,  0.0f,  // Pico Bajo
		 -0.5f, -0.0f,  -0.5f,      0.990f, 0.013f,  0.0f,  -1.0f,  0.0f,
		 -0.5f, -0.0f,   0.5f,      0.594f, 0.013f,  0.0f,  -1.0f,  0.0f,

		 // derecha abajo
		 0.0f, -0.5f,   0.0f,		0.399f, 0.336f,  0.0f,  -1.0f,  0.0f,  // Pico Bajo
		 0.5f, -0.0f,   0.5f,       0.594f, 0.013f,  0.0f,  -1.0f,  0.0f,
		 0.5f, -0.0f,  -0.5f,       0.201f, 0.013f,  0.0f,  -1.0f,  0.0f,
	};

	// Arreglo de �ndices para formar los tri�ngulos
	unsigned int dado_indices[] = {
		// Cara 1
		0, 1, 2,

		// Cara2
		3,4,5,

		// Cara3
		6,7,8,

		// Cara 4
		9,10,11,

		// Cara 5
		12,13,14,

		// Cara 6
		15,16,17,

		// Cara 7
		18,19,20,

		// Cara 8
		21,22,23
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(dado_vertices, dado_indices, 192, 24); 
	meshList.push_back(dado);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearDado8Caras();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();


	// Mi Practica6
	dadoTexture = Texture("Textures/E06-319157293_dado.jpg");
	dadoTexture.LoadTexture();
	
	DadoOcta = Texture("Textures/P06-319157293_octaedro.jpg");
	DadoOcta.LoadTexture();
	
	Dado_M = Model();
	Dado_M.LoadModel("Models/E06-319157293_dado.obj");

	Car = Model();
	Car.LoadModel("Models/P06-319157293_car.obj");

	Car_Chasis = Model();
	Car_Chasis.LoadModel("Models/P06-319157293_chasis.obj");

	Car_Faros = Model();
	Car_Faros.LoadModel("Models/P06-319157293_faros.obj");

	Car_Tire = Model();
	Car_Tire.LoadModel("Models/P06-319157293_llantaTexturizada.obj");

	Car_Trunk = Model();
	Car_Trunk.LoadModel("Models/P06-319157293_cofreTexturizado.obj");

	Car_Parabrisas = Model();
	Car_Parabrisas.LoadModel("Models/P06-319157293_parabrisasTexturizado.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes.
		//Dado de OpenGL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		//meshList[4]->RenderMesh();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado.
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Dado_M.RenderModel();
		

		
		//Reporte de pr�ctica :

		//Dado 8 Caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DadoOcta.UseTexture();
		meshList[5]->RenderMesh();
		
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 10.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Car.RenderModel();
		Car_Parabrisas.RenderModel();

		model = glm::translate(model, glm::vec3(0.798f, 0.598f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Car_Trunk.RenderModel();

		// LLANTA DELANTERA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.28f, -0.05f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Car_Tire.RenderModel();


		// LLANTA TRASERA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.26f, -0.05f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Car_Tire.RenderModel();


		// LLANTA DELANTERA IZQUIERDA
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, -0.05f, -0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Car_Tire.RenderModel();


		// LLANTA TRASERA IZQUIERDA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.25f, -0.05f, -0.88f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Car_Tire.RenderModel();

		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		Car_Chasis.RenderModel();

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		Car_Faros.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/
/*
Práctica 7: Iluminación 1 
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

// Mi Práctica 7
Model Car;
Model Car_Parabrisas;
Model Car_Chasis;
Model Car_Faros;
Model Car_Tire;
Model Car_Trunk;
Model Lampara;
Model Blackhawk_M;
Model Cristal;
Texture DadoOcta;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

PointLight copiepointLights[MAX_POINT_LIGHTS];
SpotLight copiespotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CrearDado8Caras()
{
	// Vértices del dado
	GLfloat dado_vertices[] = {

		// x      y      z			S      T		NX      NY      NZ

		   // frontal arriba
		   0.0f,  0.5f,   0.0f,		0.390f, 1.000f,  0.0f,  0.0f,  -1.0f,  // Pico Alto
		  -0.5f, -0.0f,   0.5f,     0.197f, 0.668f,  0.0f,  0.0f,  -1.0f,
		   0.5f, -0.0f,   0.5f,     0.592f, 0.668f,  0.0f,  0.0f,  -1.0f,

		   // trasera arriba
		   0.0f,  0.5f,   0.0f,		0.390f, 1.000f,  0.0f,  0.0f,  1.0f,  // Pico Alto
		  -0.5f, -0.0f,  -0.5f,     0.197f, 0.668f,  0.0f,  0.0f,  1.0f,
		   0.5f, -0.0f,  -0.5f,     0.003f, 1.000f,  0.0f,  0.0f,  1.0f,

		   // izquierda arriba
		   0.0f,  0.5f,   0.0f,		0.786f, 0.999f,  1.0f,  0.0f,  0.0f,  // Pico Alto
		  -0.5f, -0.0f,  -0.5f,     0.390f, 1.000f,  1.0f,  0.0f,  0.0f,
		  -0.5f, -0.0f,   0.5f,     0.592f, 0.668f,  1.0f,  0.0f,  0.0f,

		  // derecha arriba
		  0.0f,  0.5f,   0.0f,		0.592f, 0.668f,  -1.0f,  0.0f,  0.0f,  // Pico Alto
		  0.5f, -0.0f,   0.5f,      0.197f, 0.668f,  -1.0f,  0.0f,  0.0f,
		  0.5f, -0.0f,  -0.5f,      0.399f, 0.336f,  -1.0f,  0.0f,  0.0f,

		  // frontal abajo
		  0.0f, -0.5f,   0.0f,		0.594f, 0.013f,  0.0f,  0.0f,  -1.0f,  // Pico Bajo
		 -0.5f, -0.0f,   0.5f,      0.399f, 0.336f,  0.0f,  0.0f,  -1.0f,
		  0.5f, -0.0f,   0.5f,      0.791f, 0.336f,  0.0f,  0.0f,  -1.0f,

		  // trasera abajo
		  0.0f, -0.5f,   0.0f,		0.592f, 0.668f,  0.0f,  0.0f,  1.0f, // Pico Bajo
		 -0.5f, -0.0f,  -0.5f,      0.399f, 0.336f,  0.0f,  0.0f,  1.0f,
		  0.5f, -0.0f,  -0.5f,      0.791f, 0.336f,  0.0f,  0.0f,  1.0f,

		  // izquierda abajo
		  0.0f, -0.5f,   0.0f,		0.791f, 0.336f,  1.0f,  0.0f,  0.0f,  // Pico Bajo
		 -0.5f, -0.0f,  -0.5f,      0.990f, 0.013f,  1.0f,  0.0f,  0.0f,
		 -0.5f, -0.0f,   0.5f,      0.594f, 0.013f,  1.0f,  0.0f,  0.0f,

		 // derecha abajo
		 0.0f, -0.5f,   0.0f,		0.399f, 0.336f,  -1.0f,  0.0f,  0.0f,  // Pico Bajo
		 0.5f, -0.0f,   0.5f,       0.594f, 0.013f,  -1.0f,  0.0f,  0.0f,
		 0.5f, -0.0f,  -0.5f,       0.201f, 0.013f,  -1.0f,  0.0f,  0.0f,
	};

	// Arreglo de índices para formar los triángulos
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

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void swapSpotLight(SpotLight * spotLights, std::vector<int> ligths, int numLights) {

	SpotLight itemAux;

	for (int i = 0; i < numLights; i++) {
		itemAux = spotLights[i];
		spotLights[i] = spotLights[ligths[i]];
		spotLights[ligths[i]] = itemAux;
	}

}

void swapPointLight(PointLight* spotLights, std::vector<int> ligths, int numLights) {

	PointLight itemAux;

	for (int i = 0; i < numLights; i++) {
		itemAux = spotLights[i];
		spotLights[i] = spotLights[ligths[i]];
		spotLights[ligths[i]] = itemAux;
	}

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado8Caras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	// Mi Práctica 7
	DadoOcta = Texture("Textures/P06-319157293_octaedro.jpg");
	DadoOcta.LoadTexture();

	Car = Model();
	Car.LoadModel("Models/P07-319157293/P06-319157293_car.obj");

	Car_Chasis = Model();
	Car_Chasis.LoadModel("Models/P07-319157293/P06-319157293_chasis.obj");

	Car_Faros = Model();
	Car_Faros.LoadModel("Models/P07-319157293/P06-319157293_faros.obj");

	Car_Tire = Model();
	Car_Tire.LoadModel("Models/P07-319157293/P06-319157293_llantaTexturizada.obj");

	Car_Trunk = Model();
	Car_Trunk.LoadModel("Models/P07-319157293/P06-319157293_cofreTexturizado.obj");

	Car_Parabrisas = Model();
	Car_Parabrisas.LoadModel("Models/P07-319157293/P06-319157293_parabrisasTexturizado.obj");

	Lampara = Model();
	Lampara.LoadModel("Models/P07-lampara.obj");

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Cristal = Model();
	Cristal.LoadModel("Models/P08-cristal.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual (Luz de Lampara)
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		5.5f, 5.0f,         // Ambiental | Difuso
		0.0f, 5.0f, 8.0f,   // Posicion 
		0.3f, 0.2f, 0.1f);  // NO deben ser 0
	pointLightCount++;

	// Brillo de Cristal
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		5.0f, 2.0f,          // Ambiental | Difuso
		0.0f, 3.0f, 30.0f,   // Posicion 
		0.3f, 0.0f, 0.1f);   // NO deben ser 0
	pointLightCount++;


	unsigned int spotLightCount = 0;
	// Helicoptero
	spotLights[0] = SpotLight(1.0f, 1.0f, 0.0f,
					20.0f, 10.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, -1.0f, 0.0f,
					0.3f, 0.8f, 0.0f,
					25.0f);
	spotLightCount++;

	// Luz Faro Delantero
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 1.0f,              // Intensidades
		12.5f, 3.1f, 6.5f,       // Posicion
		5.0f, -1.0f, 0.0f,       // Direccion
		0.9f, 0.3f, 0.0f,        // No pueden ser 0 (atenuacion) 
		5.0f);					 // Angulo de apertura
	spotLightCount++;

	////linterna
	//spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f, //DIR
	//	1.0f, 0.0f, 0.0f,
	//	5.0f); //ANGULO DE APERTURA
	//spotLightCount++;


	// Luz Faro Trasero
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 1.0f,              // Intensidades
		12.5f, 3.1f, 6.5f,       // Posicion
		5.0f, -1.0f, 0.0f,       // Direccion
		0.9f, 0.3f, 0.0f,        // No pueden ser 0 (atenuacion) 
		5.0f);
	spotLightCount++;

	// Luz de cofre
	spotLights[3] = SpotLight(1.0f, 0.0f, 1.0f,
		0.5f, 1.0f,              // Intensidades
		0.0f, 0.0f, 0.0f,       // Posicion
		0.0f, 0.0f, 0.0f,       // Direccion
		0.0f, 0.0f, 0.7f,        // No pueden ser 0 (atenuacion) 
		5.0f);					 // Angulo de apertura hacia donde apunta
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	std::vector<int> lightsIndex;

	////Loop mientras no se cierra la ventana
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		// Sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz.
		spotLights[0].SetFlash(glm::vec3(-40 + mainWindow.getmuevex_helicop(), 30.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0)); // Helicoptero
		spotLights[1].SetFlash(glm::vec3(32.5 + mainWindow.getmuevex(), 3.5f, 6.3f), glm::vec3(1.0, -0.5, 0.0)); // Faro Delantero Auto 
		spotLights[2].SetFlash(glm::vec3(10.0 + mainWindow.getmuevex(), 3.5f, 6.3f), glm::vec3(-1.0, -0.5, 0.0)); // Faro Trasero Auto
		spotLights[3].SetFlash(glm::vec3(27.798 + mainWindow.getmuevex(), 2.8f, 10.85f), glm::vec3(1.0, 1.0, 0.0)); // Luz Cofre

		copiespotLights[0] = spotLights[0]; // Luz Helicóptero
		copiespotLights[1] = spotLights[1]; // Faro Delantero Auto
		copiespotLights[2] = spotLights[2]; // Faro Trasero Auto
		copiespotLights[3] = spotLights[3]; // Luz Cofre

		copiepointLights[0] = pointLights[0]; // Luz Lampara
		copiepointLights[1] = pointLights[1]; // Luz Cristal

		////Linterna de camara
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[2].SetFlash(lowerLight, camera.getCameraDirection()); //LINTERNA

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		// Para prender y apagar luz puntual (Lampara o Cristal)
		if (!mainWindow.getcntrlLuzCristal() && mainWindow.getcntrlLuzLamp()) {
			lightsIndex.push_back(0);
			swapPointLight(copiepointLights, lightsIndex, lightsIndex.size());
			lightsIndex.clear();

			shaderList[0].SetPointLights(copiepointLights, pointLightCount - 1);
		}
		else if (mainWindow.getcntrlLuzCristal() && !mainWindow.getcntrlLuzLamp()) {
			lightsIndex.push_back(1);
			swapPointLight(copiepointLights, lightsIndex, lightsIndex.size());
			lightsIndex.clear();

			shaderList[0].SetPointLights(copiepointLights, pointLightCount - 1);
		}
		else if (mainWindow.getcntrlLuzCristal() && mainWindow.getcntrlLuzLamp()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (!mainWindow.getcntrlLuzCristal() && !mainWindow.getcntrlLuzLamp()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}

			
		switch (mainWindow.getIdLuz()) {
				// Luces inciales (Solo Luz Helicoptero Encendida)
		case -1: 
			lightsIndex.push_back(0);
			if (mainWindow.getrotCofre() > 0) lightsIndex.push_back(3);
			swapSpotLight(copiespotLights, lightsIndex, lightsIndex.size());
			shaderList[0].SetSpotLights(copiespotLights, lightsIndex.size());

			lightsIndex.clear();
			break;

		case 1:
			lightsIndex.push_back(0);
			lightsIndex.push_back(1);
			if (mainWindow.getrotCofre() > 0) lightsIndex.push_back(3);
			swapSpotLight(copiespotLights, lightsIndex, lightsIndex.size());
			shaderList[0].SetSpotLights(copiespotLights, lightsIndex.size());

			lightsIndex.clear();
			break;

		case 2:
			lightsIndex.push_back(0);
			lightsIndex.push_back(2);
			if (mainWindow.getrotCofre() > 0) lightsIndex.push_back(3);
			swapSpotLight(copiespotLights, lightsIndex, lightsIndex.size());
			shaderList[0].SetSpotLights(copiespotLights, lightsIndex.size());

			lightsIndex.clear();
			break;

		case 3:
			lightsIndex.push_back(0);
			lightsIndex.push_back(3);
			swapSpotLight(copiespotLights, lightsIndex, lightsIndex.size());
			shaderList[0].SetSpotLights(copiespotLights, lightsIndex.size());

			lightsIndex.clear();
			break;
		}


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// Instacnia Dado Caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 5.0f, 10.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		DadoOcta.UseTexture();
		meshList[4]->RenderMesh();


		// Instancia Cristal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cristal.RenderModel();

		// Instancia de Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Lampara.RenderModel();

		// Instancia Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40 + mainWindow.getmuevex_helicop(), 40.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();
	
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 1.2f, 10.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		Car.RenderModel();
		Car_Parabrisas.RenderModel();

		model = glm::translate(model, glm::vec3(0.798f, 0.598f, 0.01f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotCofre()), glm::vec3(0.0f, 0.0f, 1.0f));
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

		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		Car_Faros.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

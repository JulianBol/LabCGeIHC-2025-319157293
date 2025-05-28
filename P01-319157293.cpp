#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.0f,0.0f,0.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {
		// x, y, z coordenadas

		 // Letra J
		-0.7,0.5,0.0, // A
		-0.4,0.5,0.0, // B
		-0.4,0.4,0.0, // D
		-0.7,0.5,0.0, // A
		-0.4,0.4,0.0, // D
		-0.7,0.4,0.0, // C
		-0.6,0.4,0.0, // E
		-0.5,0.5,0.0, // G
		-0.6,0.1,0.0, // F
		-0.5,0.5,0.0, // G
		-0.6,0.1,0.0, // F
		-0.5,0.1,0.0, // H
		-0.7,0.1,0.0, // I
		-0.5,0.0,0.0, // K
		-0.5,0.1,0.0, // H
		-0.7,0.1,0.0, // I
		-0.5,0.0,0.0, // K
		-0.7,0.0,0.0, // J

		// Letra B
		-0.323,0.0,0.0, // A
		-0.323,0.501,0.0, // B
		-0.252,0.0,0.0, // I
		-0.252,0.501,0.0, // H
		-0.323,0.501,0.0, // B
		-0.252,0.0,0.0, // I
		-0.252,0.449,0.0, // J
		-0.252,0.501,0.0, // H
		-0.046,0.449,0.0, // K
		-0.252,0.501,0.0, // H
		-0.046,0.449,0.0, // K
		-0.046,0.501,0.0, // C
		-0.120,0.449,0.0, // R
		-0.044,0.300,0.0, // D
		-0.046,0.449,0.0, // K
		-0.119,0.300,0.0, // S
		-0.120,0.449,0.0, // R
		-0.044,0.300,0.0, // D
		-0.119,0.244,0.0, // E
		-0.119,0.300,0.0, // S
		-0.044,0.300,0.0, // D
		-0.048,0.201,0.0, // F
		-0.119,0.201,0.0, // Q
		-0.119,0.244,0.0, // E
		-0.117,0.053,0.0, // P
		-0.047,0.053,0.0, // M
		-0.048,0.201,0.0, // F
		-0.048,0.201,0.0, // F
		-0.119,0.201,0.0, // Q
		-0.117,0.053,0.0, // P
		-0.046,0.0,0.0, // G
		-0.252,0.053,0.0, // O
		-0.047,0.053,0.0, // M
		-0.046,0.0,0.0, // G
		-0.252,0.053,0.0, // O
		-0.252,0.0,0.0, // I

		-0.253,0.335,0.0, // V
		-0.119,0.335,0.0, // W
		-0.255,0.170,0.0, // T
		-0.118,0.170,0.0, // U
		-0.119,0.335,0.0, // W
		-0.258,0.170,0.0, // T

		// Letra G
		0.333,0.501,0.0, // T
		0.333,0.434,0.0, // Q1
		0.171,0.500,0.0, // I1
		0.065,0.436,0.0, // J1
		0.333,0.434,0.0, // Q1
		0.171,0.500,0.0, // I1
		0.136,0.435,0.0, // R1
		0.065,0.102,0.0, // K1
		0.065,0.436,0.0, // J1
		0.136,0.102,0.0, // S1
		0.136,0.435,0.0, // R1
		0.065,0.102,0.0, // K1
		0.095,0.036,0.0, // L1
		0.136,0.102,0.0, // S1
		0.065,0.102,0.0, // K1
		0.157,0.0,0.0, // M1
		0.095,0.036,0.0, // L1
		0.136,0.102,0.0, // S1
		0.2,0.0,0.0, // D2
		0.136,0.102,0.0, // S1
		0.157,0.0,0.0, // M1
		0.356,0.194,0.0, // C2
		0.357,0.041,0.0, // O1
		0.2,0.0,0.0, // D2
		0.357,0.041,0.0, // O1
		0.2,0.0,0.0, // D2
		0.3,0.0,0.0, // N1
		0.248,0.060,0.0, // E2
		0.2,0.0,0.0, // D2
		0.160,0.062,0.0, // T1
		0.278,0.060,0.0, // U1
		0.278,0.150,0.0, // V1
		0.356,0.194,0.0, // C2
		0.278,0.150,0.0, // V1
		0.356,0.194,0.0, // C2
		0.321,0.227,0.0, // B2
		0.278,0.150,0.0, // V1
		0.321,0.227,0.0, // B2
		0.250,0.170,0.0, // W1
		0.321,0.227,0.0, // B2
		0.250,0.170,0.0, // W1
		0.202,0.229,0.0, // A2
	    0.250, 0.170, 0.0, // W1
		0.202, 0.229, 0.0, // A2
		0.200,0.169,0.0, // Z1
		0.136,0.382,0.0, // F2
		0.202,0.435,0.0, // G2
		0.136, 0.435, 0.0, // R1
	};

	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "PRACTICA 1: Introduccion a OpenGL", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tríangulo
	CrearTriangulo();
	CompileShaders();

	srand(time(NULL));
	float rojo = (rand() % 100) / 100.0;
	float verde = (rand() % 100) / 100.0;
	float azul = (rand() % 100) / 100.0;


	double tiempoActual = glfwGetTime();

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{

		//Recibir eventos del usuario
		glfwPollEvents();

		// tiempo para controlar el cambio de color en pantalla

		//Limpiar la ventana
		if (glfwGetTime() - tiempoActual >= 2) {

			rojo = (rand() % 100) / 100.0;
			verde = (rand() % 100) / 100.0;
			azul = (rand() % 100) / 100.0;

			tiempoActual = glfwGetTime();
		}
		glClearColor(rojo, verde, azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 106);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}
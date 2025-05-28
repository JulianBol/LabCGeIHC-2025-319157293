#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;

}
MeshColor::MeshColor()
{
	VAO = 0;
	VBO = 0;
	vertexCount = 0;

}


void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numberOfIndices)
{

	indexCount = numberOfIndices;
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tama�o, los datos y en este caso es est�tico pues no se modificar�n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::CreateMeshGeometry(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices, unsigned int numOfVertices, unsigned int numberOfIndices)
{

	indexCount = numberOfIndices;
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, &vertices[0], GL_STATIC_DRAW); //pasarle los datos al VBO asignando tama�o, los datos y en este caso es est�tico pues no se modificar�n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::CreateMeshGeometryMulticolor(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices, unsigned int numOfVertices, unsigned int numberOfIndices)
{
	indexCount = numberOfIndices;

	glGenVertexArrays(1, &VAO); // Crear VAO
	glBindVertexArray(VAO); // Asignar VAO

	// Crear y llenar el buffer de �ndices (elementos)
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, &indices[0], GL_STATIC_DRAW);

	// Crear y llenar el buffer de v�rtices (posiciones + colores)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices * 6, &vertices[0], GL_STATIC_DRAW);

	// Posici�n (primer atributo en `layout(location = 0)`)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Color (segundo atributo en `layout(location = 1)`)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void MeshColor::CreateMeshColor(GLfloat *vertices, unsigned int numOfVertices)
{
	vertexCount = numOfVertices;
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tama�o, los datos y en este caso es est�tico pues no se modificar�n los valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, (void*)(sizeof(vertices[0]) * 3));//Offset de los 3 v�rtices para tomar los 3 de color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::RenderMesh()
{
	////////////Para dibujar desde los �ndices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshColor::RenderMeshColor()
{
	////////////Para dibujar desde los v�rtices
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

void Mesh::RenderMeshGeometry()
{
	////////////Para dibujar desde los �ndices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::RenderMeshGeometryAristas()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//void Mesh::RenderMeshGeometry()
//{
//	// Dibujar la pir�mide normalmente con relleno
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Relleno normal
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.0, 1.0); // Evita solapamiento con las l�neas
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	glDisable(GL_POLYGON_OFFSET_FILL);
//
//	// Dibujar los bordes en negro
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Modo l�neas
//	glLineWidth(5.0f); // Grosor de las l�neas
//	glColor3f(0.0f, 0.0f, 0.0f); // Color negro
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // Dibujar bordes
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	// Restaurar a modo normal
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}




void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO); ///LIMPIAR BUFFER PARAE EVITAR OCUPAR LA MEMORIA
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); ///LIMPIAR BUFFER PARAE EVITAR OCUPAR LA MEMORIA
		VBO = 0;
	}
	if(VAO!=0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

void MeshColor::ClearMeshColor()
{
	
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); ///LIMPIAR BUFFER PARAE EVITAR OCUPAR LA MEMORIA
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	vertexCount= 0;
}
Mesh::~Mesh()
{
	ClearMesh();
}
MeshColor::~MeshColor()
{
	ClearMeshColor();
}

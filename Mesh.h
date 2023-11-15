#pragma once
#include <glad/glad.h>
#include "Texture.h"
#include "Material.h"

typedef struct Mesh
{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	unsigned verticesCount;

	Material material;
} Mesh;

void MeshCreate(Mesh* self, float* vertices, unsigned* indices, unsigned verticesCount, unsigned indicesCount);
/**
* very simple implementation of an obj importer, faces in the obj HAVE TO be at the end of the file
*/
void MeshCreateFromObj(Mesh* self, const char* objfilepath);
void MeshDestroy(Mesh* self);
void MeshDraw(Mesh* self, Shader* shader);


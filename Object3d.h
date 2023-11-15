#pragma once
#include "glmath.h"
#include "Mesh.h"
#include "Shader.h"

typedef struct Object3d
{
	Mesh* meshPtr;

	vec4 position;
	vec4 scale;
	vec4 rotationAxis;
	float rotationAngle;

}Object3d;

void Object3dCreate(Object3d* self, Mesh* mesh, vec4 position, vec4 scale, vec4 rotationAxis, float rotationAngle);
void Object3dDraw(Object3d* self, Shader* shader);

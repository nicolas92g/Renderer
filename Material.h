#pragma once
#include "glmath.h"
#include "Texture.h"
#include "Shader.h"

typedef struct Material
{
	vec3 diffuse;
	Texture diffuseTexture;
	int isDiffuseTextured;

} Material;

void MaterialCreate(Material* self);

void MaterialSetDiffuseTexture(Material* self, const char* filepath);
void MaterialSetDiffuseConst(Material* self, vec3 diffuseCst);

void MaterialSendToShader(Material* self, Shader* shader);

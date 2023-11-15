#pragma once
#include <glad/glad.h>

typedef struct Renderer2d
{
	GLuint squareVAO;


} Renderer2d;

void Renderer2dCreate(Renderer2d* self);
void Renderer2dDestroy(Renderer2d* self);

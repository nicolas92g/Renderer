#include "Renderer2d.h"

void Renderer2dCreate(Renderer2d* self)
{
	float vertices[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f
	};
	
	glGenVertexArrays(1, &self->squareVAO);
	glBindVertexArray(self->squareVAO);

	
}

void Renderer2dDestroy(Renderer2d* self)
{

}

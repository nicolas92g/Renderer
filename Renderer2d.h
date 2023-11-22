#pragma once
#include <glad/glad.h>
#include "glmath.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "textRenderer.h"

#define BUTTON_COLOR ((vec4){.5f, .5f, .5f, .8f})

#define BOTTOM_LEFT 0
#define BOTTOM_RIGHT 1
#define TOP_LEFT 2
#define TOP_RIGHT 3

typedef struct Renderer2d
{
	Window* windowPtr;
	Shader shader;

	TextRenderer textRenderer;
	vec4 textColor;
	
	GLuint squareVAO;
	GLuint squareVBO;

	mat4 orthoProj;

	int centerPoint;
	
} Renderer2d;

typedef struct Object2dData {
	vec2 position;
	vec2 scale;
	float angle;
}Object2dData;

typedef struct ButtonColors {
	vec4 color;
	vec4 hoverColor;
	vec4 clickedColor;
}ButtonColors;

void Renderer2dCreate(Renderer2d* self, Window* window);
void Renderer2dDestroy(Renderer2d* self);

void Renderer2dUpdate(Renderer2d* self);
void Renderer2dSetCenterPoint(Renderer2d* self, int CENTER_POINT);

void Renderer2dImage(Renderer2d* self, Texture* image, Object2dData* parameters);
void Renderer2dColor(Renderer2d* self, vec4 color, Object2dData* parameters);

int Renderer2dColorButton(Renderer2d* self, const char* text, ButtonColors colors, Object2dData* parameters);

void Renderer2dText(Renderer2d* self, const char* text, int x, int y, int scale);
void Renderer2dTextCentered(Renderer2d* self, const char* text, int x, int y, int scale);


void Object2dDataCreate(Object2dData* data, vec2 position, vec2 scale);

ButtonColors ButtonColor(vec4 color);

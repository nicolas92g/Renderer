#pragma once
#include "Window.h"
#include "Shader.h"

typedef struct Character {
	unsigned int width;
	unsigned int height;
	unsigned int offsetX;
	unsigned int offsetY;
	unsigned int advance;
	unsigned int texture;
} Character;

typedef struct TextRenderer
{
	Shader shader;
	Window* windowPtr;

	Character* loadedCharacters;

	unsigned VAO;
	unsigned VBO;

}TextRenderer;



void TextRendererCreate(TextRenderer* self, Window* window, const char* fontFilePath);
void TextRendererDestroy(TextRenderer* self);

void TextRendererDraw(TextRenderer* self, const char* text, int x, int y, int scale, vec4 color);
int TextRendererGetTextWidth(TextRenderer* self, const char* text, int scale);
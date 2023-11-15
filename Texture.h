#pragma once

typedef struct Texture
{
	unsigned id;
	int width;
	int height;
	int nbrOfChannels;

}Texture;

void TextureCreate(Texture* self, const char* filepath);
void TextureBind(Texture* self);
void TextureDestroy(Texture* self);
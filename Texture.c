#include "Texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void TextureCreate(Texture* self, const char* filepath)
{
	unsigned char* data = stbi_load(filepath, &self->width, &self->height, &self->nbrOfChannels, 0);

	glGenTextures(1, &self->id);
	glBindTexture(GL_TEXTURE_2D, self->id);

	if (!data) {
		printf("failed to load a texture at : %s \n", filepath);
		return;
	}

	GLuint format;
	switch (self->nbrOfChannels) {
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			printf("Error : texture has %d channels !\n", self->nbrOfChannels);
			return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, format, self->width, self->height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureBind(Texture* self)
{
	glBindTexture(GL_TEXTURE_2D, self->id);
}

void TextureDestroy(Texture* self)
{
	glDeleteTextures(1, &self->id);
}



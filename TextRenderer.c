#include "TextRenderer.h"
#include "Config.h"
#include "glmath.h"

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define TEXT_RENDERER_CHARACTER_RESOLUTION 128

void TextRendererCreate(TextRenderer* self, Window* window, const char* fontFilePath) {
    //characters array
    self->loadedCharacters = (Character*)malloc(256 * sizeof(Character));
    if (!self->loadedCharacters) {
        printf("TextRendererCreate failed : allocation of loadedCharacters failed !\n");
        return;
    }

    self->windowPtr = window;

    //create the shader for text rendering
    ShaderCreate(&self->shader, 
        RENDERER_SHADERS_PATH"textRenderer.vert", 
        RENDERER_SHADERS_PATH"textRenderer.frag"
    );

    FT_Library freetype;
    FT_Face face;

    //initialize freetype
    if (FT_Init_FreeType(&freetype)) {
        printf("Initialization of freetype failed\n");
        exit(EXIT_FAILURE);
    }

    //load font file
    if (FT_New_Face(freetype, fontFilePath, 0, &face)) {
        printf("font loading failed : %s\n", fontFilePath);
        exit(EXIT_FAILURE);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Set_Pixel_Sizes(face, 0, TEXT_RENDERER_CHARACTER_RESOLUTION);

    for (short c = 0; c < 256; c++)
    {
        //load the character C
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("le chargement du charactere : %d n'a pas reussie\n", c);
            continue;
        }

        Character C;

        //creation of the texture of the char
        glGenTextures(1, &C.texture);
        glBindTexture(GL_TEXTURE_2D, C.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        //texture settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //settings of the char
        C.offsetX = face->glyph->bitmap_left;
        C.offsetY = face->glyph->bitmap_top;
        C.width = face->glyph->bitmap.width;
        C.height = face->glyph->bitmap.rows;
        C.advance = face->glyph->advance.x;

        //add character to the list
        self->loadedCharacters[c] = C;
    }

    //free freetype stuff
    FT_Done_Face(face);
    FT_Done_FreeType(freetype);

    //Vertex array for the square for one character
    glGenVertexArrays(1, &self->VAO);
    glBindVertexArray(self->VAO);

    glGenBuffers(1, &self->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindVertexArray(0);
}

void TextRendererDestroy(TextRenderer* self)
{
    free(self->loadedCharacters);
    glDeleteBuffers(1, &self->VBO);
    glDeleteVertexArrays(1, &self->VAO);
}

void TextRendererDraw(TextRenderer* self, const char* text, int x, int y, int scaleInt, vec4 color){

    ShaderUse(&self->shader);
    glBindVertexArray(self->VAO);
    glActiveTexture(GL_TEXTURE0);

    float scale = scaleInt / (float)TEXT_RENDERER_CHARACTER_RESOLUTION;

    int width, height;
    glfwGetWindowSize(self->windowPtr->ptr, &width, &height);

    mat4 projection = orthographicProjection(0, width, 0, height, -10, 10);

    ShaderSetMat4(&self->shader, "projection", &projection);
    ShaderSetVec4(&self->shader, "textColor", color);

    for (short i = 0; i < strlen(text); i++)
    {
        const Character C = self->loadedCharacters[text[i]];

        const float positionX = x + C.offsetX * scale;
        const float positionY = y - (C.height - C.offsetY) * scale;

        const float l = C.width * scale;
        const float h = C.height * scale;

        float vertices[6][4] = {
            { positionX,     positionY + h,   0.0f, 0.0f },
            { positionX,     positionY,       0.0f, 1.0f },
            { positionX + l, positionY,       1.0f, 1.0f },

            { positionX,     positionY + h,   0.0f, 0.0f },
            { positionX + l, positionY,       1.0f, 1.0f },
            { positionX + l, positionY + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, C.texture);
        glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (C.advance >> 6) * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

int TextRendererGetTextWidth(TextRenderer* self, const char* text, int scaleInt) {
    float scale = scaleInt / (float)TEXT_RENDERER_CHARACTER_RESOLUTION;

    int ret = 0;

    for (short i = 0; i < strlen(text); i++)
        ret += (self->loadedCharacters[text[i]].advance >> 6) * scale;

    return ret;
}

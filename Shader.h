#pragma once
#include <glad/glad.h>
#include "glmath.h"

typedef struct Shader
{
	GLuint id;
} Shader;

void ShaderCreate(Shader* self, const char* vertexPath, const char* fragmentPath);
void ShaderDestroy(Shader* self);

void ShaderUse(Shader* self);
void ShaderSetMat4(Shader* self, const char* name, mat4* mat);
void ShaderSetInt(Shader* self, const char* name, int value);
void ShaderSetVec3(Shader* self, const char* name, vec3 value);
void ShaderSetVec4(Shader* self, const char* name, vec4 value);
void ShaderSetFloat(Shader* self, const char* name, float value);





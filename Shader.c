#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>

char* fileToString(const char* filename);

void ShaderCreate(Shader* self, const char* vertexPath, const char* fragmentPath)
{
    //get text in files
    const char* vertexText = fileToString(vertexPath);
    const char* fragmentText = fileToString(fragmentPath); 

    //create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexText, NULL);
    glCompileShader(vertexShader);//compile vertex shader

    free(vertexText);

    int success;
    char InfoCompilation[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//get copilation info

    if (!success)//check compilation
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, InfoCompilation);
        printf("%s\n", InfoCompilation);
    }

    //create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentText, NULL);
    glCompileShader(fragmentShader);//compile fragment shader

    free(fragmentText);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//get compilation info 

    if (!success)//check compilation
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, InfoCompilation);
        printf("%s\n", InfoCompilation);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//create program

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);//add both shaders to the program

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);//check success
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, InfoCompilation);
        printf("%s\n", InfoCompilation);
        exit(EXIT_FAILURE);
    }

    glUseProgram(shaderProgram);//load this new shader

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);//clean

    self->id = shaderProgram;
}

void ShaderDestroy(Shader* self)
{
    glDeleteProgram(self->id);
}

void ShaderUse(Shader* self)
{
    glUseProgram(self->id);
    
}

void ShaderSetMat4(Shader* self, const char* name, mat4* mat)
{
    ShaderUse(self);
    glUniformMatrix4fv(glGetUniformLocation(self->id, name), 1, GL_FALSE, &mat->col0.x);
}

void ShaderSetInt(Shader* self, const char* name, int value)
{
    ShaderUse(self);
    glUniform1i(glGetUniformLocation(self->id, name), value);
}

void ShaderSetVec3(Shader* self, const char* name, vec3 value)
{
    ShaderUse(self);
    glUniform3fv(glGetUniformLocation(self->id, name), 1,  &value.x);
}

char* fileToString(const char* filename) {

    FILE* file;
    fopen_s(&file, filename, "r");
    if (!file) return NULL;

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);

    char* source = calloc(1, size);
    if (!source || !size) return NULL;

    rewind(file);
    fread(source, size, sizeof(char), file);

    fclose(file);

    return source;
}
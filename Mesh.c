#include "Mesh.h"
#include "glmath.h"
#include "Config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//private function
void ImportMaterialFromObjMtl(Mesh* self, const char* mtlFile, const char* objfilepath);


void MeshCreate(Mesh* self, float* vertices, unsigned* indices, unsigned verticesCount, unsigned indicesCount)
{
    //create vertex array
    glGenVertexArrays(1, &self->VAO);
    glBindVertexArray(self->VAO);

    //create vertex buffer
    glGenBuffers(1, &self->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesCount * 8, vertices, GL_STATIC_DRAW);

    //create index buffer
    glGenBuffers(1, &self->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicesCount, indices, GL_STATIC_DRAW);
    
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    //unbind vertex array 
    glBindVertexArray(0);

    self->verticesCount = indicesCount;
}

void MeshCreateFromObj(Mesh* self, const char* objfilepath)
{
    //count number of v, vn, vt and f in the file
    unsigned numberOfVertices = 0, numberOfNormals = 0, numberOfUvs = 0, numberOfFaces = 0;

    FILE* file = fopen(objfilepath, "r");
    if (!file) {
        printf("failed to import : %s\n", objfilepath);
        return;
    }

    while (1) {
        char word[200];
        int res = fscanf(file, "%s", word);
        if (res == EOF) break;
        word[199] = 0;
        if (!strcmp(word, "v")) numberOfVertices++;
        else if (!strcmp(word, "vn")) numberOfNormals++;
        else if (!strcmp(word, "vt")) numberOfUvs++;
        else if (!strcmp(word, "f")) numberOfFaces++;
        //import the material
        else if (!strcmp(word, "mtllib")) {

            const int r = fscanf(file, "%s", word);

            ImportMaterialFromObjMtl(self, word, objfilepath);
        }
    }

    vec3* vertices = (vec3*)malloc(numberOfVertices * sizeof(vec3));
    vec3* normals = (vec3*)malloc(numberOfNormals * sizeof(vec3));
    vec2* uvs = (vec2*)malloc(numberOfUvs * sizeof(vec2));
    

    //get the number of vertices
    self->verticesCount = numberOfFaces * 3;

    //now read the obj file
    fseek(file, 0, SEEK_SET);//set the cursor at the begining

    //data to fill with the imported mesh
    float* verticesData = (float*)malloc(self->verticesCount * sizeof(float) * 8);
    unsigned* indicesData = (unsigned*)malloc(self->verticesCount * sizeof(unsigned));

    //verify mallocs
    if (!vertices || !normals || !uvs || !verticesData) {
        printf("Error while importing : %s, failed allocation occured!\n", objfilepath);
        return;
    }

    //cursors to fill arrays
    unsigned verticesCursor = 0, normalsCursor = 0, uvsCursor = 0, verticesDataCursor = 0, indicesDataCursor = 0;
    
    while (1) {
        char buf[200];
        const int res = fscanf(file, "%s", buf);
        if (res == EOF) break;
        buf[199] = 0;

        if (!strcmp(buf, "v")) {
            const vec3* v = &vertices[verticesCursor];
            const int r = fscanf(file, "%f %f %f\n", &v->x, &v->y, &v->z);
            verticesCursor++;
        }
        else if (!strcmp(buf, "vn")) {
            const vec3* vn = &normals[normalsCursor];
            const int r = fscanf(file, "%f %f %f\n", &vn->x, &vn->y, &vn->z);
            normalsCursor++;
        }
        else if (!strcmp(buf, "vt")) {
            const vec2* vt = &uvs[uvsCursor];
            const int r = fscanf(file, "%f %f\n", &vt->x, &vt->y);
            uvsCursor++;
        }
        else if (!strcmp(buf, "f")) {
            unsigned vIndex[3], uvIndex[3], nIndex[3];

            const int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                vIndex + 0, uvIndex + 0, nIndex + 0,
                vIndex + 1, uvIndex + 1, nIndex + 1,
                vIndex + 2, uvIndex + 2, nIndex + 2
            );

            if (matches != 9) printf("Error importing %s, some faces don't have 9 indices.\n", objfilepath);

            for (short i = 0; i < 3; i++)
            {
                const vec3 pos = vertices[vIndex[i] - 1];
                const vec3 normal = normals[nIndex[i] - 1];
                const vec2 uv = uvs[uvIndex[i] - 1];

                //add positions
                verticesData[verticesDataCursor++] = pos.x;
                verticesData[verticesDataCursor++] = pos.y;
                verticesData[verticesDataCursor++] = pos.z;
                //add normals
                verticesData[verticesDataCursor++] = normal.x;
                verticesData[verticesDataCursor++] = normal.y;
                verticesData[verticesDataCursor++] = normal.z;
                //add uvs
                verticesData[verticesDataCursor++] = uv.x;
                verticesData[verticesDataCursor++] = uv.y;
            }
        }
    }

    for (unsigned i = 0; i < self->verticesCount; i++) indicesData[i] = i;

    MeshCreate(self, verticesData, indicesData, self->verticesCount, self->verticesCount);
    
    fclose(file);

    free(verticesData);
    free(indicesData);
    free(vertices);
    free(normals);
    free(uvs);
}

void MeshDestroy(Mesh* self)
{
    glDeleteBuffers(1, &self->VBO);
    glDeleteBuffers(1, &self->EBO);
    glDeleteVertexArrays(1, &self->VAO);
}

void MeshDraw(Mesh* self, Shader* shader)
{
    ShaderUse(shader);

    //bind material
    MaterialSendToShader(&self->material, shader);

    //draw geometry
    glBindVertexArray(self->VAO);
    glDrawElements(GL_TRIANGLES, self->verticesCount, GL_UNSIGNED_INT, (void*)0);
}

void ImportMaterialFromObjMtl(Mesh* self, const char* mtlFile, const char* objfilepath) {
    //init material
    MaterialCreate(&self->material);

    //strings
    char path[201] = { 0 };
    char mtlPath[201] = { 0 };
    //add the path of the obj file to the mtl file
    char* lastSlashPtr = strrchr(objfilepath, '/');
    if (lastSlashPtr) {
        size_t lastSlashIndex = (lastSlashPtr - objfilepath) / sizeof(char);
        strncpy(path, objfilepath, lastSlashIndex + 1);
        path[200] = 0;
    }
    strcpy(mtlPath, path);
    strcat(mtlPath, mtlFile);

    //check that the mtl file is open
    FILE* file = fopen(mtlPath, "r");
    if (!file) {
        printf("ERROR : failed to open the .mtl file linked to : %s at the path : %s\n", objfilepath, mtlPath);
        return;
    }

    //read the mtl file
    while (1) {
        char word[200];
        int res = fscanf(file, "%s", word);
        if (res == EOF) break;
        word[199] = 0;
        if (!strcmp(word, "map_Kd")) {
            int r = fscanf(file, "%s", word);
            char imgPath[201];
            strcpy(imgPath, path);
            strcat(imgPath, word);

            MaterialSetDiffuseTexture(&self->material, imgPath);
        }
        else if (!strcmp(word, "Kd")) {
            int r = fscanf(file, "%f %f %f\n", &self->material.diffuse.x, &self->material.diffuse.y, &self->material.diffuse.z);
        }
    }
    
}

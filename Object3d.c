#include "Object3d.h"

void Object3dCreate(Object3d* self,Mesh* mesh, vec4 position, vec4 scale, vec4 rotationAxis, float rotationAngle)
{
    self->meshPtr = mesh;
    self->position = position;
    self->scale = scale;
    self->rotationAngle = rotationAngle;
    self->rotationAxis = rotationAxis;
}

void Object3dDraw(Object3d* self, Shader* shader)
{

    const mat4 rot = rotationMatrix(self->rotationAxis, self->rotationAngle);
    const mat4 trans = translationMatrix(self->position.x, self->position.y, self->position.z);
    const mat4 scale = scaleMatrix(self->scale.x, self->scale.y, self->scale.z);
    const mat4 tmp = Mat4Multiply(&trans, &rot);
    const mat4 model = Mat4Multiply(&tmp, &scale);

    ShaderSetMat4(shader, "model", &model);

	MeshDraw(self->meshPtr, shader);
}

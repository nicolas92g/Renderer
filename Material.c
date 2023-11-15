#include "Material.h"

void MaterialCreate(Material* self)
{
	self->isDiffuseTextured = 0;
	self->diffuse = (vec3){ 1.f, 1.f, 1.f };
}

void MaterialSetDiffuseTexture(Material* self, const char* filepath)
{
	self->isDiffuseTextured = 1;
	TextureCreate(&self->diffuseTexture, filepath);
}

void MaterialSetDiffuseConst(Material* self, vec3 diffuseCst)
{
	//remove texture if it exist
	if (self->isDiffuseTextured) {
		TextureDestroy(&self->diffuseTexture);
		self->isDiffuseTextured = 0;
	}
	self->diffuse = diffuseCst;
}

void MaterialSendToShader(Material* self, Shader* shader)
{
	if (self->isDiffuseTextured) {
		glActiveTexture(GL_TEXTURE0);
		TextureBind(&self->diffuseTexture);
		ShaderSetInt(shader, "diffuseTexture", 0);
		ShaderSetVec3(shader, "diffuseConst", (vec3){-1.f});
	}
	else {
		ShaderSetVec3(shader, "diffuseConst", self->diffuse);
	}
}


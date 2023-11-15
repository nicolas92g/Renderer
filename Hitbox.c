#include "Hitbox.h"

void HitboxCreate(Hitbox* self, float sphereRadius, vec3 offset)
{
	self->offset = offset;
	self->shape.radius = sphereRadius;
	self->shape.center = make_vec3f(0);
}

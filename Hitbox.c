#include "Hitbox.h"

void HitboxCreate(Hitbox* self, float sphereRadius, vec3 offset)
{
	self->position = offset;
	self->shape.radius = sphereRadius;
	self->shape.center = make_vec3f(0);
}

void HitboxSetPosition(Hitbox* self, vec3 position)
{
	self->shape.center = position;
}

void HitboxSetShape(Hitbox* self, Sphere shape)
{
	self->shape = shape;
}

int HitboxCollide(Hitbox* self, const Hitbox* other)
{
	return Vec3Distance(self->position, other->position) <= self->shape.radius + other->shape.radius;
}



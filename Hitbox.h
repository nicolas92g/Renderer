#pragma once
#include "glmath.h"

typedef struct Sphere{
	float radius;
	vec3 center;
}Sphere;

typedef struct Hitbox
{
	Sphere shape;
	vec3 position;
} Hitbox;

void HitboxCreate(Hitbox* self, float sphereRadius, vec3 offset);

void HitboxSetPosition(Hitbox* self, vec3 position);
void HitboxSetShape(Hitbox* self, Sphere shape);

int HitboxCollide(Hitbox* self, const Hitbox* other);

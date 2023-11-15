#pragma once
#include "glmath.h"

typedef struct Sphere{
	float radius;
	vec3 center;
}Sphere;

typedef struct Hitbox
{
	Sphere shape;
	vec3 offset;
} Hitbox;

void HitboxCreate(Hitbox* self, float sphereRadius, vec3 offset);


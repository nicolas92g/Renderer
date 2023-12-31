#pragma once
#include "glmath.h"
#include "Window.h"
#include "Shader.h"

typedef struct Camera
{
	const Window* window;
	

	vec4 position;
	vec4 look;
	vec4 upVector;

	float fov;
	float zNear;
	float zFar;

	float vAngle;
	float hAngle;
}Camera;

void CameraCreate(Camera* self, const Window* window, vec4 position, float fov);
/**
 * @brief create the matrices and then send them to the shader
 */
void CameraUpdate(Camera* self, Shader* shader);
/**
 * allow to control camera with zqsd-lctrl-space and mouse.
 */
void CameraBasicControls(Camera* self, float sensivity, float speed, float sprintMultiplier);





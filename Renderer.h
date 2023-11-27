#pragma once
#include "Window.h"
#include "Shader.h"
#include "Object3d.h"
#include "Camera.h"
#include "Material.h"
#include "Renderer2d.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define RENDERER_MAX_OBJECT3D_NUMBER 1000

typedef struct Renderer
{
	Window* windowPtr;
	Camera* cameraPtr;

	Shader mainShader;
	Object3d** objectsPtrs;
	unsigned numberOfObjects;

}Renderer;

/**
 * must be called first
 */
void RendererCreate(Renderer* self, Window* window, Camera* camera);
/**
 * clear all the memory allocated for the renderer
 */
void RendererDestroy(Renderer* self);
/**
 * add an object to the rendering and false if the objects array is full or if the object is already in
 */
int RendererAdd(Renderer* self, Object3d* object);
/**
 * remove an object from the rendering return true if it was founded and removed and false if not
 */
int RendererRemove(Renderer* self, Object3d* object);
/**
 * Draw all the objects; render the scene
 */
void RendererRender(Renderer* self);
/**
 * start the rendering loop
 */
void RendererStartMainLoop(Renderer* self, void(*frameCallback)(Renderer*));





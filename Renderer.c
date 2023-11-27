#include "Renderer.h"
#include "Config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERS_SIZES 150

void RendererCreate(Renderer* self, Window* window, Camera* camera)
{
	//init ptrs
	self->windowPtr = window;
	self->cameraPtr = camera;

	//allocate empty array of objects
	self->objectsPtrs = (Object3d**)calloc(RENDERER_MAX_OBJECT3D_NUMBER, sizeof(Object3d*));
	self->numberOfObjects = 0;

	//concatenate shaders path with the shader's namefile ( ex : "C:/shaders/" + "shader.vert" ) // (for the main shader)
	char bufVert[BUFFERS_SIZES] = { 0 };
	strcpy(bufVert, RENDERER_SHADERS_PATH);
	strcat(bufVert, "basic.vert");

	char bufFrag[BUFFERS_SIZES] = { 0 };
	strcpy(bufFrag, RENDERER_SHADERS_PATH);
	strcat(bufFrag, "basic.frag");

	//create main shader
	ShaderCreate(&self->mainShader, bufVert, bufFrag);

	//transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable cull facing
	glEnable(GL_CULL_FACE);
}

void RendererDestroy(Renderer* self)
{
	//ShaderDestroy(&self->mainShader);
	free(self->objectsPtrs);
}

int RendererAdd(Renderer* self, Object3d* object)
{
	//check if array is full
	if (self->numberOfObjects == RENDERER_MAX_OBJECT3D_NUMBER) 
		return 0;

	//check it is not already in
	for (unsigned i = 0; i < self->numberOfObjects; i++)
		if (object == self->objectsPtrs[i])
			return 0;

	//add object
	self->objectsPtrs[self->numberOfObjects] = object;
	self->numberOfObjects++;

	return 1;
}

int RendererRemove(Renderer* self, Object3d* object)
{
	for (unsigned i = 0; i < self->numberOfObjects; i++)
	{
		if (object == self->objectsPtrs[i]) {
			self->numberOfObjects--;
			for (unsigned j = i; j < self->numberOfObjects; j++)
			{
				self->objectsPtrs[j] = self->objectsPtrs[j + 1];
			}
			self->objectsPtrs[self->numberOfObjects] = NULL;
		}
	}
}

void RendererRender(Renderer* self)
{
	glEnable(GL_DEPTH_TEST);

	CameraUpdate(self->cameraPtr, &self->mainShader);

	for (unsigned i = 0; i < self->numberOfObjects; i++)
	{
		Object3dDraw(self->objectsPtrs[i], &self->mainShader);
	}

	glDisable(GL_DEPTH_TEST);
}

void RendererStartMainLoop(Renderer* self, void(*frameCallback)(void*), void* frameObj)
{
	int isFullscreen = 0, previousF11state = 0;

	do {
		WindowStartFrame(self->windowPtr);

		//display frame
		RendererRender(self);

		//compute next frame
		frameCallback(self);

		//fullscreen
		if (WindowGetKey(self->windowPtr, GLFW_KEY_F11)) {
			if (previousF11state == 0) {
				isFullscreen = !isFullscreen;
				WindowSetFullscreen(self->windowPtr, isFullscreen);
			}
		}
		previousF11state = WindowGetKey(self->windowPtr, GLFW_KEY_F11);


		WindowEndFrame(self->windowPtr);
	} while (!WindowShouldClose(self->windowPtr));
}

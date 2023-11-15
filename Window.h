#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

typedef struct Window{
	GLFWwindow* ptr;

	double previousTime;
	double deltaTime;
} Window;

void WindowCreate(Window* self, unsigned width, unsigned height, const char* title);
void WindowDestroy(Window* self);

void WindowStartFrame(Window* self);
void WindowEndFrame(Window* self);
int WindowShouldClose(Window* self);
void WindowSetBackgroundColor(float red, float green, float blue, float alpha);
int WindowGetWidth(const Window* self);
int WindowGetHeight(const Window* self);
void WindowSetTitle(Window* self, const char* title);
int WindowGetKey(Window* self, int GLFW_KEY);
void WindowSetIcon(Window* self, const char* filepath);

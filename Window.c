#include "Window.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stb_image.h>

void WindowCreate(Window* self, unsigned width, unsigned height, const char* title)
{
    glfwInit();//initialize glfw

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//using OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);//multisampling with 16 samples per pixels

    self->ptr = glfwCreateWindow(width, height, title, NULL, NULL);//create the window

    //check that the window was created sucessfully
    if (self->ptr == NULL)
    {
        printf("Window could not be created\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //create the opengl context
    glfwMakeContextCurrent(self->ptr);

    //initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad failed to initialize\n");
        exit(EXIT_FAILURE);
    }

    //set viewport size to the window size
    glViewport(0, 0, width, height);

    //set input mode
    glfwSetInputMode(self->ptr, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
}

void WindowDestroy(Window* self)
{
    glfwDestroyWindow(self->ptr);
    glfwTerminate();
}

void WindowStartFrame(Window* self)
{
    int width, height;
    glfwGetWindowSize(self->ptr, &width, &height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void WindowEndFrame(Window* self)
{
    self->deltaTime = glfwGetTime() - self->previousTime;
    self->previousTime = glfwGetTime();

    glfwPollEvents();
    glfwSwapBuffers(self->ptr);
}

int WindowShouldClose(Window* self) {
    return glfwWindowShouldClose(self->ptr);
}

void WindowSetBackgroundColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
}

int WindowGetWidth(const Window* self)
{
    int ret;
    glfwGetWindowSize(self->ptr, &ret, NULL);
    return ret;
}

int WindowGetHeight(const Window* self)
{
    int ret;
    glfwGetWindowSize(self->ptr, NULL, &ret);
    return ret;
}

void WindowSetTitle(Window* self, const char* title)
{
    glfwSetWindowTitle(self->ptr, title);
}

int WindowGetKey(Window* self, int GLFW_KEY)
{
    return glfwGetKey(self->ptr, GLFW_KEY);
}

void WindowSetIcon(Window* self, const char* filepath)
{
    GLFWimage icon;
    icon.pixels = stbi_load(filepath, &icon.width, &icon.height, NULL, 4);
    glfwSetWindowIcon(self->ptr, 1, &icon);
}

#include "Camera.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

void CameraCreate(Camera* self, const Window* window, vec4 position, float fov)
{
	self->window = window;
	self->position = position;
	self->look = make_vec4(0, 0, 1, 1);
    self->upVector = make_vec4(0, 1, 0, 1);
	self->fov = fov;
    self->zNear = .1f;
    self->zFar = 200.f;
    self->hAngle = 0.f;
    self->vAngle = PI / 2.f;
    glfwSetInputMode(self->window->ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CameraUpdate(Camera* self, Shader* shader)
{
    mat4 projection = perspectiveProjection(
        (float)WindowGetWidth(self->window) / (float)WindowGetHeight(self->window),
        self->fov, 
        self->zNear,
        self->zFar
    );

    mat4 view = viewMatrix(self->position, Vec4Add(self->position, self->look), self->upVector);

    mat4 idm = identityMatrix(1);

    ShaderSetMat4(shader, "projection", &projection);
    ShaderSetMat4(shader, "view", &view);
}

void CameraBasicControls(Camera* self, float sensivity, float speed, float sprintMultiplier)
{
    //enable/disable controls with TAB
    static int disableControl = 0, previousKeyState = 0;

    
    if (WindowGetKey(self->window, GLFW_KEY_TAB)) {
        if (!previousKeyState) {
            disableControl = !disableControl;
            if (disableControl)
                glfwSetInputMode(self->window->ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(self->window->ptr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
    }

    previousKeyState = WindowGetKey(self->window, GLFW_KEY_TAB);

    if (disableControl) return;

    //compute middle of the screen
    double middleX = WindowGetWidth(self->window) / 2, middleY = WindowGetHeight(self->window) / 2;

    //get movement of the mouse for the frame
    double xMouse, yMouse;
    glfwGetCursorPos(self->window->ptr, &xMouse, &yMouse);
    double offsetX = xMouse - middleX, offsetY = yMouse - middleY;

    
    //force mouse at the middle of the screen
    glfwSetCursorPos(self->window->ptr, middleX, middleY);

    //printf("%f %f\n", offsetX, offsetY);

    //update angles of the cam
    self->hAngle -= sensivity * offsetX;
    self->vAngle -= sensivity * offsetY;

    const float limit = PI / 2.f - .1f;
    if (self->vAngle <= -limit)
        self->vAngle = -limit;
    else if (self->vAngle >= limit)
        self->vAngle = limit;

    self->look = make_vec4(
        cos(self->vAngle) * sin(self->hAngle),
        sin(self->vAngle),
        cos(self->vAngle) * cos(self->hAngle),
        1
    );

    vec4 direction = self->look;
    direction.y = 0;
    direction = normalize(direction);

    vec4 right = normalize(crossProduct(self->look, self->upVector));

    speed *= self->window->deltaTime;

    //keyboard control
    if (WindowGetKey(self->window, GLFW_KEY_LEFT_SHIFT))
        speed *= sprintMultiplier;

    if (WindowGetKey(self->window, GLFW_KEY_SPACE))
        self->position = Vec4Add(self->position, Vec4Multiplyf(self->upVector, speed));

    if (WindowGetKey(self->window, GLFW_KEY_LEFT_CONTROL))
        self->position = Vec4Add(self->position, Vec4Multiplyf(self->upVector, -speed));

    if (WindowGetKey(self->window, GLFW_KEY_W))
        self->position = Vec4Add(self->position, Vec4Multiplyf(direction, speed));

    if (WindowGetKey(self->window, GLFW_KEY_S))
        self->position = Vec4Add(self->position, Vec4Multiplyf(direction, -speed));

    if (WindowGetKey(self->window, GLFW_KEY_D))
        self->position = Vec4Add(self->position, Vec4Multiplyf(right, speed));

    if (WindowGetKey(self->window, GLFW_KEY_A))
        self->position = Vec4Add(self->position, Vec4Multiplyf(right, -speed));
}

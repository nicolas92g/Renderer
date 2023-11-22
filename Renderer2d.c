#include "Renderer2d.h"
#include "Config.h"

#include <stdio.h>

#define BUFFERS_SIZES 200

void drawCall(Renderer2d* self);
void makeModelMatrix(Renderer2d* self, Object2dData* parameters);
Object2dData ModifyObject2dDataCenterPoint(Renderer2d* self, Object2dData* data);

void Renderer2dCreate(Renderer2d* self, Window* window)
{
	//vars init
	self->centerPoint = BOTTOM_LEFT;
	self->textColor = make_vec4(0, 0, 0, 1);

	//text
	TextRendererCreate(&self->textRenderer, window, "./assets/Roboto-Medium.ttf");

	//window
	self->windowPtr = window;

	// VAO
	float vertices[] = {
		-.5f, -.5f,
		 .5f, -.5f,
		-.5f,  .5f,

		-.5f,  .5f,
		 .5f, -.5f,
		 .5f,  .5f
	};
	
	glGenVertexArrays(1, &self->squareVAO);
	glBindVertexArray(self->squareVAO);

	glGenBuffers(1, &self->squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, self->squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * 3, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0);

	//shader
	char bufVert[BUFFERS_SIZES] = { 0 };
	strcpy(bufVert, RENDERER_SHADERS_PATH);
	strcat(bufVert, "2d.vert");

	char bufFrag[BUFFERS_SIZES] = { 0 };
	strcpy(bufFrag, RENDERER_SHADERS_PATH);
	strcat(bufFrag, "2d.frag");

	ShaderCreate(&self->shader, bufVert, bufFrag);
}

void Renderer2dDestroy(Renderer2d* self)
{
	glDeleteVertexArrays(1, &self->squareVAO);
	glDeleteBuffers(1, &self->squareVBO);
	ShaderDestroy(&self->shader);

	TextRendererDestroy(&self->textRenderer);
}

void Renderer2dUpdate(Renderer2d* self)
{
	self->orthoProj = orthographicProjection(0, WindowGetWidth(self->windowPtr), 0, WindowGetHeight(self->windowPtr), -10, 10);
	ShaderSetMat4(&self->shader, "projection", &self->orthoProj);
}

void Renderer2dSetCenterPoint(Renderer2d* self, int CENTER_POINT)
{
#	ifndef NDEBUG
	if (CENTER_POINT < 0 || CENTER_POINT > 3) 
		printf("Warning ! : Renderer2dSetCenterPoint received an incorrect value : %d\n", CENTER_POINT);
#	endif // !NDEBUG

	self->centerPoint = CENTER_POINT;
}

void Renderer2dImage(Renderer2d* self, Texture* image, Object2dData* parameters)
{
	Object2dData updatedData = ModifyObject2dDataCenterPoint(self, parameters);
	makeModelMatrix(self, &updatedData);

	ShaderSetVec4(&self->shader, "colorConst", (vec4) { -1 });

	glActiveTexture(GL_TEXTURE0);
	ShaderSetInt(&self->shader, "imageTexture", 0);
	TextureBind(image);

	drawCall(self);
}

void Renderer2dColor(Renderer2d* self, vec4 color, Object2dData* parameters)
{
	Object2dData updatedData = ModifyObject2dDataCenterPoint(self, parameters);
	makeModelMatrix(self, &updatedData);

	glActiveTexture(GL_TEXTURE0);
	ShaderSetVec4(&self->shader, "colorConst", color);

	drawCall(self);
}

int Renderer2dColorButton(Renderer2d* self, const char* text, ButtonColors colors, Object2dData* parameters)
{
	Object2dData updatedData = ModifyObject2dDataCenterPoint(self, parameters);
	double x, y;
	glfwGetCursorPos(self->windowPtr->ptr, &x, &y);
	y = WindowGetHeight(self->windowPtr) - y;

	const double buttonHalfWidth = updatedData.scale.x / 2.;
	const double buttonHalfHeight = updatedData.scale.y / 2.;
	const double buttonX = updatedData.position.x;
	const double buttonY = updatedData.position.y;

	//test if mouse hover the button
	const int hover = (x >= buttonX - buttonHalfWidth && x <= buttonX + buttonHalfWidth) && (y >= buttonY - buttonHalfHeight && y <= buttonY + buttonHalfHeight);
	const int clicked = glfwGetMouseButton(self->windowPtr->ptr, GLFW_MOUSE_BUTTON_1);

	if (clicked && hover) {
		Renderer2dColor(self, colors.clickedColor, parameters);
	}	
	if (hover)
		Renderer2dColor(self, colors.hoverColor, parameters);
	else
		Renderer2dColor(self, colors.color, parameters);

	float textYoffset = parameters->scale.y / 10.f * ((self->centerPoint == BOTTOM_RIGHT || self->centerPoint == BOTTOM_LEFT) ? 1 : -1);
	Renderer2dTextCentered(self, text, parameters->position.x, parameters->position.y + textYoffset, parameters->scale.y / 1.5);
	
	return clicked && hover;
}

void Renderer2dText(Renderer2d* self, const char* text, int x, int y, int scale)
{
	if (!self->centerPoint)
		TextRendererDraw(&self->textRenderer, text, x, y, scale, self->textColor);

	else if (self->centerPoint == BOTTOM_RIGHT) {
		const int w = TextRendererGetTextWidth(&self->textRenderer, text, scale);
		TextRendererDraw(&self->textRenderer, text, WindowGetWidth(self->windowPtr) - w - x,  y, scale, self->textColor);
	}

	else if (self->centerPoint == TOP_LEFT) {
		TextRendererDraw(&self->textRenderer, text, x, WindowGetHeight(self->windowPtr) - scale - y, scale, self->textColor);
	}

	else if (self->centerPoint == TOP_RIGHT) {
		const int w = TextRendererGetTextWidth(&self->textRenderer, text, scale);
		TextRendererDraw(&self->textRenderer, 
			text, 
			WindowGetWidth(self->windowPtr) - w - x,
			WindowGetHeight(self->windowPtr) - scale - y, 
			scale, 
			self->textColor
		);
	}

#	ifndef NDEBUG
	else printf("Warning : Renderer2dText : Renderer2d.centerPoint is incorrect : %d\n", self->centerPoint);
#	endif // !NDEBUG
}

void Renderer2dTextCentered(Renderer2d* self, const char* text, int x, int y, int scale)
{
	const int w = TextRendererGetTextWidth(&self->textRenderer, text, scale);
	Renderer2dText(self, text, x - w / 2.f, y - scale / 2.f, scale);
}

void Object2dDataCreate(Object2dData* data, vec2 position, vec2 scale)
{
	data->position = position;
	data->scale = scale;
	data->angle = 0;
}

Object2dData ModifyObject2dDataCenterPoint(Renderer2d* self, Object2dData* data) {
	Object2dData ret = *data;
	if (!self->centerPoint) return ret;//BOTTOM_LEFT

	else if (self->centerPoint == BOTTOM_RIGHT || self->centerPoint == TOP_RIGHT) {
		ret.position.x = WindowGetWidth(self->windowPtr) - ret.position.x;
	}

	if (self->centerPoint == TOP_LEFT || self->centerPoint == TOP_RIGHT) {
		ret.position.y = WindowGetHeight(self->windowPtr) - ret.position.y;
	}
	return ret;
}

ButtonColors ButtonColor(vec4 color)
{
	const float hoverOffset = .1f, clickedOffset = .2f;
	return (ButtonColors) {
		color,
		Vec4Add(color, make_vec4(hoverOffset, hoverOffset, hoverOffset, 0)),
		Vec4Add(color, make_vec4(clickedOffset, clickedOffset, clickedOffset, 0))
	};
}

void makeModelMatrix(Renderer2d* self, Object2dData* parameters) {
	
	const mat4 rot = rotationMatrix(make_vec4(0, 0, 1, 1), parameters->angle / 180 * PI);
	const mat4 trans = translationMatrix(parameters->position.x, parameters->position.y, 0);
	const mat4 scaleMat = scaleMatrix(parameters->scale.x, parameters->scale.y, 1);
	const mat4 tmp = Mat4Multiply(&trans, &rot);
	const mat4 model = Mat4Multiply(&tmp, &scaleMat);

	ShaderSetMat4(&self->shader, "model", &model);
}

void drawCall(Renderer2d* self) {

	ShaderUse(&self->shader);

	glBindVertexArray(self->squareVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

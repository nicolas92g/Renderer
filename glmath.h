/*****************************************************************//**
 * \file   glmath.h
 * \brief  some mathematicals functions for 3D
 * \author nicolas GUILLOT
 * \date   May 2021
 *********************************************************************/

#ifndef GLMATH
#define GLMATH

#include <math.h>

#define PI 3.141592

/**
 * @brief define a vector of 4 floats
 */
typedef struct vector4 {
	float x;
	float y;
	float z;
	float w;
} vec4;
/**
 * @brief define a vector of 3 floats
 */
typedef struct vector3 {
	float x;
	float y;
	float z;
} vec3;
/**
 * @brief define a vector of 2 floats
 */
typedef struct vector2 {
	float x;
	float y;
} vec2;
/**
 * @brief define a matrix 4x4 
 */
typedef struct matrix4x4 {
	vec4 col0;
	vec4 col1;
	vec4 col2;
	vec4 col3;
} mat4;

/**
 * create a vec3.
 */
vec3 make_vec3(float x, float y, float z);
/**
 * create a vec3 with one value
 */
vec3 make_vec3f(float x);
/**
 * @brief create a vector
 */
vec4 make_vec4(float x, float y, float z, float w);
/**
 * @brief fill a vec4 with one value
 */
vec4 make_vec4f(float x);
/**
 * @brief multiply a matrix by a float
 */
vec4 Vec4Multiplyf(vec4 vec, float f);
/**
 * @brief add two vectors
 */
vec4 Vec4Add(vec4 v1, vec4 v2);
/**
 * @brief create an orthographic projection matrix (adapted to 2D)
 */
mat4 orthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar);
/**
 * @brief create a matrix that is able to simulate a 3D world
 */
mat4 perspectiveProjection(float aspect, float fov, float zNear, float zFar);
/**
 * @brief create an identity matrix
 */
mat4 identityMatrix(float factor);
/**
 * @brief rescale matrix
 */
mat4 scaleMatrix(float x, float y, float z);
/**
 * @brief translation matrix
 */
mat4 translationMatrix(float x, float y, float z);
/**
 * @brief rotation matrix
 */
mat4 rotationMatrix(vec4 axe, float angle);
/**
 * @brief multiply two matrices together
 */
mat4 Mat4Multiply(mat4* m1, mat4* m2);
/**
 * @brief view matrix
 */
mat4 viewMatrix(const vec4 eyePos, const vec4 lookAt, const vec4 up);
/**
 * @brief give the cross product of two vectors
 */
vec4 crossProduct(vec4 A, vec4 B);
/**
 * @brief give dot or scalar product of two vectors
 */
float dotProduct(vec4 A, vec4 B);
/**
 * @brief modifie un vecteur pour que sa norme soit egale a 1
 */
vec4 normalize(vec4 a);

#endif //GLMATH
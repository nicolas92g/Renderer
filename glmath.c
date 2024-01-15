#include "glmath.h"

float square(const float a) {
    return a * a;
}

mat4 orthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
    mat4 ret = identityMatrix(1);

    ret.col0.x = 2.0f / (right - left);
    ret.col1.y = 2.0f / (top - bottom);
    ret.col2.z = -2.0f / (zFar - zNear);

    ret.col3.x = -(right + left) / (right - left);
    ret.col3.y = -(top + bottom) / (top - bottom);
    ret.col3.z = -(zFar + zNear) / (zFar - zNear);

    return ret;
}

vec2 make_vec2(float x, float y)
{
    return (vec2) { x, y};
}

vec2 make_vec2f(float x)
{
    return (vec2) { x, x };
}

vec3 make_vec3(float x, float y, float z)
{
    return (vec3) { x, y, z };
}

vec3 make_vec3f(float x)
{
    return (vec3) { x, x, x };
}

vec4 make_vec4(float x, float y, float z, float w) {
    return (vec4) { x, y, z, w };
}

vec4 make_vec4f(float x)
{
    return (vec4) { x, x, x, x };
}

vec4 Vec4Multiplyf(vec4 vec, float f)
{
    return (vec4) { vec.x * f, vec.y * f, vec.z * f, vec.w * f };
}

vec4 Vec4Add(vec4 v1, vec4 v2)
{
    return (vec4) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

vec4 Vec4Sub(vec4 v1, vec4 v2)
{
    return (vec4) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

mat4 identityMatrix(float f) {
    mat4 ret;

    ret.col0 = make_vec4(f, 0, 0, 0);
    ret.col1 = make_vec4(0, f, 0, 0);
    ret.col2 = make_vec4(0, 0, f, 0);
    ret.col3 = make_vec4(0, 0, 0, f);
    return ret;
}

mat4 scaleMatrix(float x, float y, float z) {
    mat4 ret;
    
    ret.col0 = make_vec4(x, 0, 0, 0);
    ret.col1 = make_vec4(0, y, 0, 0);
    ret.col2 = make_vec4(0, 0, z, 0);
    ret.col3 = make_vec4(0, 0, 0, 1);
    return ret;
}

mat4 translationMatrix(float x, float y, float z) {
    mat4 ret;
    
    ret.col0 = make_vec4(1, 0, 0, 0);
    ret.col1 = make_vec4(0, 1, 0, 0);
    ret.col2 = make_vec4(0, 0, 1, 0);
    ret.col3 = make_vec4(x, y, z, 1);
    return ret;
}

mat4 rotationMatrix(vec4 u, float A)
{
    if (u.x == .0f && u.y == .0f && u.z == .0f || A == .0f) return identityMatrix(1);

    u = normalize(u);

    const float cosA = cos(A);
    const float sinA = sin(A);
    const float IcosA = 1.0f - cosA;

    mat4 ret;
    ret.col0 = make_vec4(cosA + square(u.x) * IcosA     , u.x * u.y * IcosA - u.z * sinA,   u.x * u.z * IcosA + u.y * sinA, 0);
    ret.col1 = make_vec4(u.y * u.x * IcosA + u.z * sinA , cosA + square(u.y) * IcosA    ,   u.y * u.z * IcosA - u.x * sinA, 0);
    ret.col2 = make_vec4(u.z * u.x * IcosA - u.y * sinA , u.z * u.y * IcosA + u.x * sinA,   cosA + square(u.z) * IcosA    , 0);
    ret.col3 = make_vec4(0, 0, 0, 1);
    return ret;
}

mat4 Mat4Multiply(mat4* m1, mat4* m2)
{
    const vec4 SrcA0 = m1->col0;
    const vec4 SrcA1 = m1->col1;
    const vec4 SrcA2 = m1->col2;
    const vec4 SrcA3 = m1->col3;

    const vec4 SrcB0 = m2->col0;
    const vec4 SrcB1 = m2->col1;
    const vec4 SrcB2 = m2->col2;
    const vec4 SrcB3 = m2->col3;

    mat4 ret;
    ret.col0 = Vec4Add(Vec4Add(Vec4Multiplyf(SrcA0, SrcB0.x), Vec4Multiplyf(SrcA1, SrcB0.y)), Vec4Add(Vec4Multiplyf(SrcA2, SrcB0.z), Vec4Multiplyf(SrcA3, SrcB0.w)));
    ret.col1 = Vec4Add(Vec4Add(Vec4Multiplyf(SrcA0, SrcB1.x), Vec4Multiplyf(SrcA1, SrcB1.y)), Vec4Add(Vec4Multiplyf(SrcA2, SrcB1.z), Vec4Multiplyf(SrcA3, SrcB1.w)));
    ret.col2 = Vec4Add(Vec4Add(Vec4Multiplyf(SrcA0, SrcB2.x), Vec4Multiplyf(SrcA1, SrcB2.y)), Vec4Add(Vec4Multiplyf(SrcA2, SrcB2.z), Vec4Multiplyf(SrcA3, SrcB2.w)));
    ret.col3 = Vec4Add(Vec4Add(Vec4Multiplyf(SrcA0, SrcB3.x), Vec4Multiplyf(SrcA1, SrcB3.y)), Vec4Add(Vec4Multiplyf(SrcA2, SrcB3.z), Vec4Multiplyf(SrcA3, SrcB3.w)));
    return ret;
}

mat4 perspectiveProjection(float aspect, float fov, float zNear, float zFar) {
    const float tanDemiFov = tan(fov * .5);
    const float zPortee = zNear - zFar;

    mat4 ret;
    ret.col0 = make_vec4(1.0f / (aspect * tanDemiFov), 0, 0, 0);
    ret.col1 = make_vec4(0, 1.0f / tanDemiFov, 0, 0);
    ret.col2 = make_vec4(0, 0, -(-zNear - zFar) / zPortee, -1);
    ret.col3 = make_vec4(0, 0, 2.0f * zFar * zNear / zPortee, 0);
    return ret;
}

mat4 viewMatrix(const vec4 eyePos, const vec4 lookAt, const vec4 up) {
    mat4 ret = identityMatrix(1);

    const vec4 f = normalize(Vec4Add(lookAt, Vec4Multiplyf(eyePos, -1.0f)));
    const vec4 s = normalize(crossProduct(f, up));
    const vec4 u = crossProduct(s, f);


    ret.col0.x = s.x;
    ret.col1.x = s.y;
    ret.col2.x = s.z;
    ret.col0.y = u.x;
    ret.col1.y = u.y;
    ret.col2.y = u.z;
    ret.col0.z = -f.x;
    ret.col1.z = -f.y;
    ret.col2.z = -f.z;
    ret.col3.x = -dotProduct(s, eyePos);
    ret.col3.y = -dotProduct(u, eyePos);
    ret.col3.z = dotProduct(f, eyePos);

    return ret;
}

vec4 crossProduct(vec4 A, vec4 B)
{
    vec4 ret = make_vec4(0, 0, 0, 1);
    ret.x = A.y * B.z - A.z * B.y;
    ret.y = A.z * B.x - A.x * B.z;
    ret.z = A.x * B.y - A.y * B.x;
    ret.w = 1;
    return ret;
}

float dotProduct(vec4 A, vec4 B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

vec4 normalize(vec4 a) {
    return Vec4Multiplyf(a, 1.0f / sqrt((double)square(a.x) + (double)square(a.y) + (double)square(a.z)));
}

float Vec3Distance(vec3 a, vec3 b)
{
    return sqrt(
        pow(b.x - a.x, 2) + 
        pow(b.y - a.y, 2) + 
        pow(b.z - a.z, 2)
    );
}

#include "Renderer.h"


Window window;
Camera camera;

Mesh squareMesh;
Object3d floorObj;
Mesh importedMesh;
Object3d importedObj;

//computation of a frame in the main loop
void frame(Renderer* r) {
    importedObj.rotationAngle += .01;

    if (WindowGetKey(&window, GLFW_KEY_E)) RendererRemove(r, &importedObj);
    if (WindowGetKey(&window, GLFW_KEY_Q)) RendererAdd(r, &importedObj);

    CameraBasicControls(&camera, .003f, 10.f, 5);
}

//initializations before the loop
void init(Renderer* r) {

    WindowSetBackgroundColor(.3, .7, .9, 1.);

    float vertices[] = {
        //coordonnées (xyz)        //droites normales          //coordonées uv (pour les textures)
       -0.5f,  -0.5f,  0.0f,       0.0f,   0.0f, 1.0f,        0.0f,   0.0f,
        0.5f,  -0.5f,  0.0f,       0.0f,   0.0f, 1.0f,        1.0f,   0.0f,
       -0.5f,   0.5f,  0.0f,       0.0f,   0.0f, 1.0f,        0.0f,   1.0f,
        0.5f,   0.5f,  0.0f,       0.0f,   0.0f, 1.0f,        1.0f,   1.0f,
    };

    unsigned indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    //3D Objects
    MeshCreate(&squareMesh, vertices, indices, 4, 6);

    Object3dCreate(&floorObj, &squareMesh, make_vec4f(0), make_vec4(200,200, 1, 1), make_vec4(1, 0, 0, 1), PI / 2.f);
    RendererAdd(r, &floorObj);

    MeshCreateFromObj(&importedMesh, "assets/codechatLogo.obj");
    //MeshCreateFromObj(&importedMesh, "assets/cube.obj");

    Object3dCreate(&importedObj, &importedMesh, make_vec4(0, 15, 4, 1), make_vec4f(10), make_vec4(0, 1, 0, 1), 0);
    RendererAdd(r, &importedObj);

    MaterialCreate(&squareMesh.material);
    MaterialSetDiffuseTexture(&squareMesh.material, "assets/enzo.png");
}

//free everything after the loop
void end() {
    MeshDestroy(&squareMesh);
}

//main function
int main() {

    WindowCreate(&window, 1000, 600, "renderer test");
    WindowSetIcon(&window, "assets/enzo.png");

    CameraCreate(&camera, &window, make_vec4(0, 10, 4, 1), 1.1);
    
    Renderer renderer;
    RendererCreate(&renderer, &window, &camera);
    RendererStartMainLoop(&renderer, init, frame, end);

    WindowDestroy(&window);
    RendererDestroy(&renderer);
    return 0;
}


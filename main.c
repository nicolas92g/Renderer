#include "Renderer.h"


Window window;
Camera camera;

Mesh squareMesh;
Object3d floorObj;
Mesh importedMesh;
Object3d importedObj;

Renderer2d r2d;

size_t f = 0;

//initializations before the loop
void init(Renderer* r) {

    WindowSetBackgroundColor(.3, .7, .9, 1.);

    float vertices[] = {
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

    Object3dCreate(&floorObj, &squareMesh, make_vec4f(0), make_vec4(200, 200, 1, 1), make_vec4(1, 0, 0, 1), PI / 2.f);
    RendererAdd(r, &floorObj);

    MeshCreateFromObj(&importedMesh, "assets/codechatLogo.obj");
    //MeshCreateFromObj(&importedMesh, "assets/cube.obj");

    Object3dCreate(&importedObj, &importedMesh, make_vec4(0, 15, 4, 1), make_vec4f(10), make_vec4(0, 1, 0, 1), 0);
    RendererAdd(r, &importedObj);

    MaterialCreate(&squareMesh.material);
    MaterialSetDiffuseTexture(&squareMesh.material, "assets/enzo.png");

    //2d
    Renderer2dCreate(&r2d, &window);
    Renderer2dSetCenterPoint(&r2d, BOTTOM_LEFT);
    r2d.textColor = make_vec4(0, 0, 0, 1);
}

//computation of a frame in the main loop
void frame(Renderer* r) {

    f++;

    importedObj.rotationAngle += 1 * window.deltaTime;

    importedObj.position.y = 6 + cos(glfwGetTime()) * 2;

    if (WindowGetKey(&window, GLFW_KEY_E)) RendererRemove(r, &importedObj);
    if (WindowGetKey(&window, GLFW_KEY_Q)) RendererAdd(r, &importedObj);

    CameraBasicControls(&camera, .003f, 10.f, 5);

    //2d
    Object2dData button, img2;
    Object2dDataCreate(&button, make_vec2(200, 200), make_vec2(200, 50));
    Object2dDataCreate(&img2, make_vec2(110, 110), make_vec2(100, 100));

    Renderer2dUpdate(&r2d);
    //Renderer2dImage(&r2d, &squareMesh.material.diffuseTexture, &img1);
    //Renderer2dColor(&r2d, make_vec4(1, 0, 0, .3), &img2);
    if (Renderer2dColorButton(&r2d, "test button", ButtonColor(BUTTON_COLOR), &button)) {
        WindowSetFullscreen(&window, 1);
    }

    
   
    Renderer2dText(&r2d, "ceci est du texte !", 10, 10, 20);
}


//free everything after the loop
void end() {
    MeshDestroy(&squareMesh);
    Renderer2dDestroy(&r2d);
}

//main function
int main() {

    WindowCreate(&window, 1000, 600, "renderer test");
    WindowSetIcon(&window, "assets/enzo.png");

    CameraCreate(&camera, &window, make_vec4(0, 10, 1, 5), 1.1);
    
    Renderer renderer;
    RendererCreate(&renderer, &window, &camera);
    RendererStartMainLoop(&renderer, init, frame, end);

    WindowDestroy(&window);
    RendererDestroy(&renderer);
    
    
    return 0;
}


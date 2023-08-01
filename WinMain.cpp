#include "SDL2/SDL.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring> // Agregamos esta línea para usar memcpy
#include <windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "draw.h"
#include "shapes.h" // Include the shapes header


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

Color clearColor = {61, 0, 80, 1};
Color currentColor = {255, 225, 69, 1};

// Calcular el aspect ratio
float aspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

// Definir la matriz de proyección
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

// Definir la matriz de vista
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

class Vertex {
public:
    // Constructor
    Vertex() {}

    // Constructor para vértices de tamaño 2, 3 y 4
    template<typename... Args>
    explicit Vertex(Args... args) {
        if constexpr (sizeof...(args) == 2) {
            v = glm::vec4(args..., 0.0f, 0.0f);
        } else if constexpr (sizeof...(args) == 3) {
            v = glm::vec4(args..., 0.0f);
        } else if constexpr (sizeof...(args) == 4) {
            v = glm::vec4(args...);
        } else {
            // Manejar el caso de error en tamaño del vértice aquí si lo deseas
        }
    }

    // Operador de suma
    Vertex operator+(const Vertex& other) const {
        return Vertex(v + other.v);
    }

    // Operador de multiplicación con constantes
    Vertex operator*(const float scalar) const {
        return Vertex(v * scalar);
    }

    Vertex operator*(const glm::mat4& matrix) const {
        glm::vec4 result = matrix * glm::vec4(glm::vec3(v), 1.0f);
        return Vertex(result.x, result.y, result.z, result.w);
    }


    // Función para imprimir el vértice
    void print() const {
        std::cout << "(";
        for (int i = 0; i < v.length(); ++i) {
            std::cout << v[i];
            if (i < v.length() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")" << std::endl;
    }

private:
    glm::vec4 v; // Usaremos vec4 para soportar vértices de hasta tamaño 4
};




void renderBuffer(SDL_Renderer* renderer) {
    // Crea una textura para el framebuffer
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH,
        FRAMEBUFFER_HEIGHT
    );

    // Actualiza la textura con los datos del framebuffer
    SDL_UpdateTexture(
        texture,
        NULL,
        framebuffer.data(), // Utiliza .data() para obtener el puntero al inicio del vector
        FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    // Renderiza la textura en el renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Libera la textura
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Game of Life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    glm::vec3 p1(100.0f, 100.0f, 0.0f);
    glm::vec3 p2(400.0f, 300.0f, 0.0f);
    
    // Ejemplos válidos de creación de vértices
    Vertex v1(1.0f, 2.0f);               // Vértice de tamaño 2
    Vertex v2(3.0f, 4.0f, 5.0f);         // Vértice de tamaño 3
    Vertex v3(0.1f, 0.2f, 0.3f, 0.4f);   // Vértice de tamaño 4

    // Dibujamos la línea entre p1 y p2
    //line(p1, p2);
    
    // Triángulo con coordenadas directas
    glm::vec3 A(100.0f, 100.0f, 0.0f);
    glm::vec3 B(400.0f, 100.0f, 0.0f);
    glm::vec3 C(250.0f, 400.0f, 0.0f);

    // Dibujamos el triángulo
    //triangle(A, B, C);


    std::vector<glm::vec3> vertices = {
        { 1.0,  1.0, -1.0 },  // Vértice 0
        {-1.0,  1.0, -1.0 },  // Vértice 1
        {-1.0,  1.0,  1.0 },  // Vértice 2
        { 1.0,  1.0,  1.0 },  // Vértice 3
        { 1.0, -1.0, -1.0 },  // Vértice 4
        {-1.0, -1.0, -1.0 },  // Vértice 5
        {-1.0, -1.0,  1.0 },  // Vértice 6
        { 1.0, -1.0,  1.0 }   // Vértice 7
    };

    // Ejemplo de caras que forman el cubo
    std::vector<Face> faces = {
        Face{ { {0}, {1}, {2} } }, // Cara 0: Vértice 0, Vértice 1, Vértice 2
        Face{ { {0}, {2}, {3} } }, // Cara 1: Vértice 0, Vértice 2, Vértice 3
        Face{ { {1}, {5}, {6} } }, // Cara 2: Vértice 1, Vértice 5, Vértice 6
        Face{ { {1}, {6}, {2} } }, // Cara 3: Vértice 1, Vértice 6, Vértice 2
        Face{ { {0}, {4}, {7} } }, // Cara 4: Vértice 0, Vértice 4, Vértice 7
        Face{ { {0}, {7}, {3} } }  // Cara 5: Vértice 0, Vértice 7, Vértice 3
    };


    // Definir la matriz de proyección
    float aspectRatio = 4.0f / 3.0f;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Definir la matriz de vista
    glm::vec3 cameraPosition(3.0f, 3.0f, 3.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    // Renderizar el cubo
    drawModel(vertices, faces, projectionMatrix, viewMatrix,WINDOW_WIDTH,WINDOW_HEIGHT);


    // Dibujar el cubo triangulado
    //drawModel(vertices, faces, projectionMatrix, viewMatrix);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Call our render function
        render(renderer);

        // Present the framebuffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
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
#include "shapes.h"
#include <fstream>
#include <sstream>
#include <string>
// Para tener acceso a SDL_Renderer


const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 1200;
const int RENDER_SCALE = 45;
const int RENDER_WIDTH = WINDOW_WIDTH / RENDER_SCALE;
const int RENDER_HEIGHT = WINDOW_HEIGHT / RENDER_SCALE;

SDL_Window* window;
SDL_Renderer* renderer;



struct Face {
    std::vector<std::array<int, 3>> vertexIndices;
};

void point(int x, int y, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}




void line(const Vertex& start, const Vertex& end) {
    int x1 = round(start.getX() * RENDER_SCALE) + WINDOW_WIDTH / 2;
    int y1 = round(start.getY() * RENDER_SCALE) + WINDOW_HEIGHT / 2;
    int x2 = round(end.getX() * RENDER_SCALE) + WINDOW_WIDTH / 2;
    int y2 = round(end.getY() * RENDER_SCALE) + WINDOW_HEIGHT / 2;

    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        point(x1, y1, { 255, 255, 255 });

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void triangle(const Vertex& A, const Vertex& B, const Vertex& C) {
    line(A, B);
    line(B, C);
    line(C, A);
}


bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            out_vertices.emplace_back(x, y, z);
        } else if (type == "f") {
            Face face;
            char delimiter;
            std::array<int, 3> vertexIndices;

           for (int i = 2; i >= 0; --i) {
            std::string vertexIndexStr;
            iss >> vertexIndexStr;
            std::istringstream vertexIndexStream(vertexIndexStr);
            vertexIndexStream >> vertexIndices[i];
            vertexIndices[i]--; // Subtract 1 to match C++ indexing
            vertexIndexStream >> delimiter; // Skip texture and normal indices
        }


            face.vertexIndices.emplace_back(vertexIndices);
            out_faces.push_back(face);
        }
    }

    file.close();
    return true;
}





void drawModel(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    // Dibujar las caras del cubo utilizando las funciones de línea
    for (const auto& face : faces) {
        const auto& verticesIdx = face.vertexIndices;
        for (const auto& vertexIdx : verticesIdx) {
            const glm::vec3& posA = vertices[vertexIdx[0]];
            const glm::vec3& posB = vertices[vertexIdx[1]];
            const glm::vec3& posC = vertices[vertexIdx[2]];
            Vertex A(posA.x, posA.y, posA.z);
            Vertex B(posB.x, posB.y, posB.z);
            Vertex C(posC.x, posC.y, posC.z);
            triangle(A, B, C);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Blender's Model", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool quit = false;

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    if (!loadOBJ("../navecita.obj", vertices, faces)) {
        std::cerr << "Error loading OBJ file." << std::endl;
        return 1;
    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawModel(vertices, faces);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
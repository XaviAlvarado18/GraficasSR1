#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include "SDL2/SDL.h"
#include <glm/glm.hpp>


const int FRAMEBUFFER_WIDTH = 800; // Ancho del framebuffer
const int FRAMEBUFFER_HEIGHT = 600; 


class Vertex {
public:
    glm::vec3 position;

    Vertex(float x, float y, float z) : position(x, y, z) {}

    float getX() const { return position.x; }
    float getY() const { return position.y; }
};


#endif

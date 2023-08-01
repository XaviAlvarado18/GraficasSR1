#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include "SDL2/SDL.h"
#include <glm/glm.hpp>


const int FRAMEBUFFER_WIDTH = 800; // Ancho del framebuffer
const int FRAMEBUFFER_HEIGHT = 600; 


struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

extern std::vector<Color> framebuffer; // Declaración extern de framebuffer




void point(const glm::ivec2& point);
void line(const glm::vec3& p1, const glm::vec3& p2);

#endif

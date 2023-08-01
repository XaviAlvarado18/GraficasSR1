#include <glm/glm.hpp>
#include "SDL2/SDL.h"
#include <vector>
#include "draw.h"


std::vector<Color> framebuffer(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT); // Buffer de píxeles
extern Color currentColor;



void point(const glm::ivec2& point) {
    int x = point.x;
    int y = point.y;

    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

void line(const glm::vec3& p1, const glm::vec3& p2) {
    int x0 = static_cast<int>(p1.x);
    int y0 = static_cast<int>(p1.y);
    int x1 = static_cast<int>(p2.x);
    int y1 = static_cast<int>(p2.y);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        point(glm::ivec2(x0, y0));

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
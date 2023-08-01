#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>
#include <vector>
#include <array> // Añadimos esta línea para usar std::array

struct Face {
    std::vector<std::array<int, 3>> vertexIndices;

    // Constructor para inicializar el vector de índices de vértices
    explicit Face(const std::vector<std::array<int, 3>>& indices) : vertexIndices(indices) {}
};

void triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
void drawModel(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces,  const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,int screenWidth, int screenHeight);


#endif // SHAPES_H

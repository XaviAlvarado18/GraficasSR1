#include "shapes.h"
#include "draw.h"
#include <glm/glm.hpp>


void triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    // Draw lines to form the triangle
    line(A, B);
    line(B, C);
    line(C, A);
}

glm::ivec2 project(const glm::vec3& point, const glm::mat4& projection, const glm::mat4& view, int windowWidth, int windowHeight) {
    glm::vec4 projected = projection * view * glm::vec4(point, 1.0f);

    // Ajustamos las coordenadas normalizadas al tamaño de la ventana (windowWidth x windowHeight)
    glm::ivec2 projectedCoords;
    projectedCoords.x = static_cast<int>((projected.x + 1.0f) * 0.5f * windowWidth);
    projectedCoords.y = static_cast<int>((1.0f - projected.y) * 0.5f * windowHeight);

    return projectedCoords;
}


void drawModel(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, int windowWidth, int windowHeight) {
    // Cálculo del centro de masa del cubo
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    for (const auto& vertex : vertices) {
        center += vertex;
    }
    center /= static_cast<float>(vertices.size());

    // Centramos el cubo restando el centro de masa a cada vértice
    std::vector<glm::vec3> centeredVertices = vertices;
    for (auto& vertex : centeredVertices) {
        vertex -= center;
    }

    for (const auto& face : faces) {
        if (face.vertexIndices.size() != 3) {
            // Solo trabajamos con caras triangulares, así que ignoramos las demás
            continue;
        }

        int indexA = face.vertexIndices[0][0] - 1;
        int indexB = face.vertexIndices[1][0] - 1;
        int indexC = face.vertexIndices[2][0] - 1;

        glm::vec3 A = centeredVertices[indexA];
        glm::vec3 B = centeredVertices[indexB];
        glm::vec3 C = centeredVertices[indexC];

        glm::ivec2 projectedA = project(A, projectionMatrix, viewMatrix, windowWidth, windowHeight);
        glm::ivec2 projectedB = project(B, projectionMatrix, viewMatrix, windowWidth, windowHeight);
        glm::ivec2 projectedC = project(C, projectionMatrix, viewMatrix, windowWidth, windowHeight);

        // Convertir las coordenadas proyectadas a glm::vec3
        glm::vec3 projectedAVec3(projectedA.x, projectedA.y, 0.0f);
        glm::vec3 projectedBVec3(projectedB.x, projectedB.y, 0.0f);
        glm::vec3 projectedCVec3(projectedC.x, projectedC.y, 0.0f);

        // Llamar a triangle con las coordenadas proyectadas como glm::vec3
        triangle(projectedAVec3, projectedBVec3, projectedCVec3);
    }
}

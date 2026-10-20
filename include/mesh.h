#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * Represents a single vertex of a mesh, including its position, color, normal, and UV coordinates.
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};

/**
 * Represents the raw data of a mesh, including its vertices and indices.
 */
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

/**
 * Represents a mesh in the scene, including its vertices and indices.
 */
class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void draw() const;

    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        GLsizei indexCount = 0;
};

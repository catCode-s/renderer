#pragma once

#include "scene.h"
#include "camera.h"
#include "shader.h"
#include <glm/glm.hpp>

/**
 * Represents the renderer, responsible for rendering the scene using the camera, shader, and projection matrix.
 */
class Renderer {
public:
    void render(
        const Scene& scene,
        const Camera& camera,
        Shader& shader,
        const glm::mat4& projection
    ) const;
};
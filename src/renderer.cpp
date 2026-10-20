#include "renderer.h"

/**
 * render the scene using the provided camera, shader, and projection matrix
 * @param scene the scene to be rendered
 * @param camera the camera to be used for rendering
 * @param shader the shader to be used for rendering
 * @param projection the projection matrix to be used for rendering
 */
void Renderer::render(const Scene& scene, const Camera& camera, Shader& shader, 
                      const glm::mat4& projection) const {

    shader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const glm::mat4 view = camera.getViewMatrix();
    
    for (const auto& object : scene.getObjects()) {
        const Transform& transform = object.getTransform();
        glm::mat4 model = transform.getModelMatrix();

        shader.setMat4("mvp", projection * view * model);
        shader.setMat4("model", model);

        object.getMesh().draw();
    }
}
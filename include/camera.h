#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

/**
 * A class that represents a camera in the scene
 */
class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::vec3 getCameraPosition() const;
    void cameraMovement(GLFWwindow* window, float deltaTime);
    void mouseMovement(float xpos, float ypos);

private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    float yaw;
    float pitch;
    float lastMouseX;
    float lastMouseY;
    float cameraSpeed;
    float mouseSensitivity;
};

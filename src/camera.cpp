#include "camera.h"

Camera::Camera() {
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

    yaw = -90.0f;
    pitch = 0.0f;
    lastMouseX = 400.0f;
    lastMouseY = 300.0f;
    cameraSpeed = 2.5f;
    mouseSensitivity = 0.1f;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Camera::cameraMovement(GLFWwindow* window, float deltaTime) {
    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= cameraRight * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += cameraRight * velocity;
}

void Camera::mouseMovement(float xpos, float ypos) {
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed since glfw y-coordinates go from top to bottom
    lastMouseX = xpos;
    lastMouseY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}
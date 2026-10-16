#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iterator>
#include <cmath>
#include "glm/gtc/matrix_transform.hpp"
#include "geometry/cube.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void run(GLFWwindow* window);

int main() {
    // initialize glfw platform backend
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "GLFW initialization failed\n";
        return EXIT_FAILURE;
    }

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "mouse", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // make the target to render this window
    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cerr << "Could not retrieve OpenGL function addresses\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // set the callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST); // removing this causes interesting occlusion effects

    run(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->mouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

void run(GLFWwindow* window) {
    // shader & texture setup
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    Texture texture("assets/textures/hound_in_field_square.png");
    shader.use();
    shader.setInt("texture1", 0);
    texture.bind(0);

    MeshData cubeData = createCube();
    Mesh cube(cubeData.vertices, cubeData.indices);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  0.0f, -2.0f),
        glm::vec3(-2.0f,  0.0f, -2.0f),
        glm::vec3( 0.0f,  2.0f,  2.0f),
        glm::vec3( 0.0f, -2.0f,  2.0f)
    };

    // camera setup
    Camera camera;
    glfwSetWindowUserPointer(window, &camera);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // define model-view-projection matrix
    glm::mat4 view;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 identity = glm::mat4(1.0f);

    float currentFrame, deltaTime, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glfwPollEvents();

        // set time between frames
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // update camera position and orientation based on user input
        camera.cameraMovement(window, deltaTime);
        view = camera.getViewMatrix();

        // shader uniforms
        shader.use();
        shader.setVec3("cameraPos", camera.getCameraPosition());
        glm::vec3 lightPos(sin(currentFrame) * 3.0f, 4.0f, cos(currentFrame) * 3.0f);
        shader.setVec3("lightPos", lightPos);

        // Render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::size_t i = 0; i < std::size(cubePositions); ++i) {
            glm::mat4 model =
                glm::translate(identity, cubePositions[i]) *
                glm::rotate(identity, currentFrame 
                            * glm::radians(15.0f * static_cast<float>(i)) 
                            * (i + 1), glm::vec3(0.5f, 1.0f, 0.0f));

            shader.setMat4("mvp", projection * view * model);
            shader.setMat4("model", model);

            cube.draw();
        }

        glfwSwapBuffers(window);
    }
}
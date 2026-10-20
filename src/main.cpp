#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>
#include "glm/gtc/matrix_transform.hpp"
#include "geometry/cube.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"
#include "scene.h"

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

    // run the main render loop
    run(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

/**
 * callback function for handling framebuffer size changes
 * @param window the GLFW window
 * @param width the new width of the framebuffer
 * @param height the new height of the framebuffer
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * callback function for handling cursor position changes
 * @param window the GLFW window
 * @param xpos the new x position of the cursor
 * @param ypos the new y position of the cursor
 */
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->mouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

/**
 * run the main render loop
 * @param window the GLFW window
 */
void run(GLFWwindow* window) {
    // shader & texture setup
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    Texture texture("assets/textures/hound_in_field_square.png");
    shader.use();
    shader.setInt("texture1", 0);
    texture.bind(0);

    MeshData cubeData = createCube();
    Mesh cube(cubeData.vertices, cubeData.indices);

    // create the scene with multiple cubes
    Scene scene;
    scene.addObject(cube).getTransform().position = {0.0f, 0.0f, 0.0f};
    scene.addObject(cube).getTransform().position = {2.0f, 0.0f, -2.0f};
    scene.addObject(cube).getTransform().position = {-2.0f, 0.0f, -2.0f};
    scene.addObject(cube).getTransform().position = {0.0f, 2.0f, 2.0f};
    scene.addObject(cube).getTransform().position = {0.0f, -2.0f, 2.0f};

    // camera setup
    Camera camera;
    glfwSetWindowUserPointer(window, &camera);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set up loop variables and projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    float currentFrame, deltaTime, lastFrame = 0.0f;
    glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f));
    Renderer renderer;

    // main render loop
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

        // shader uniforms
        shader.use();
        shader.setVec3("cameraPos", camera.getCameraPosition());
        glm::vec3 lightPos(sin(currentFrame) * 3.0f, 4.0f, cos(currentFrame) * 3.0f);
        shader.setVec3("lightPos", lightPos);;

        // update object rotations
        auto& objects = scene.getObjects();
        for (std::size_t i = 0; i < objects.size(); ++i) {
            Transform& transform = objects[i].getTransform();

            transform.rotation = glm::angleAxis(
                currentFrame *
                glm::radians(15.0f * static_cast<float>(i)) *
                (i + 1),
                rotationAxis
            );
        }

        // Render scene
        renderer.render(scene, camera, shader, projection);
        glfwSwapBuffers(window);
    }
}
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"
#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->mouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

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
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // set the callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

    // shader & texture setup
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    Texture texture("assets/textures/hound_in_field_square.png");
    shader.use();
    shader.setInt("texture1", 0);
    texture.bind(0);

    float vertices[] = {
        // position                // color             // UV
        // Back face
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        // Right face
        0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f
    };

    // define and upload vertex buffer data to the GPU
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // define interpretation of vertex buffer data
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnable(GL_DEPTH_TEST);

    GLuint indices[] = {
        0,   1,  2,    0,  2,  3,     // back
        4,   5,  6,    4,  6,  7,     // front
        8,   9, 10,    8, 10, 11,     // left
        12, 13, 14,   12, 14, 15,     // right
        16, 17, 18,   16, 18, 19,     // top
        20, 21, 22,   20, 22, 23      // bottom
    };

    // define and upload element buffer data to the GPU
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLsizei indexCount = std::size(indices);

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

        // update timing variables for the current frame
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // read user-input and update camera position and orientation
        camera.cameraMovement(window, deltaTime);
        view = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(std::size_t i = 0; i < std::size(cubePositions); ++i) {
            glm::mat4 model = glm::translate(identity, cubePositions[i]) 
                              * glm::rotate(identity, currentFrame * 
                                glm::radians(15.0f * static_cast<float>(i)) * (i + 1), 
                                glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setMat4("mvp", projection * view * model);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        }

        shader.setFloat("time", static_cast<float>(glfwGetTime()));
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}
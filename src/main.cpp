#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.h"

// source code for vertex shader
const char* vertexShaderSource =
    "#version 330 core\n"
    "uniform mat4 mvp;\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mvp * vec4(position, 1.0);\n"
    "    vertexColor = color;\n"    
    "}\n";

// source code for fragment shader
const char* fragmentShaderSource =
    "#version 330 core\n"
    "uniform float time;\n"
    "in vec3 vertexColor;\n"
    "out vec4 fragmentColor;\n"
    "void main()\n"
    "{\n"
    "    float red = sin(time) * 0.5 + 0.5;\n"
    "    fragmentColor = vec4(red, vertexColor.g, vertexColor.b, 1.0);\n"
    "}\n";


constexpr float CAMERA_SPEED = 2.5f;
// constexpr float WIDTH = 2.5f;
// constexpr float CAMERA_SPEED = 2.5f;
constexpr float MOUSE_SENSITIVITY = 0.1f;

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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);



    // CPU-side vertex data
    float vertices[] = {
        // Back: indices 0-3
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

        // Front: indices 4-7
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);

    GLuint indices[] = {
        // back triangles
        0, 2, 1,    0, 3, 2,
        // front triangles
        4, 5, 6,    4, 6, 7,
        // left triangles
        0, 7, 3,    0, 4, 7,
        // right
        1, 2, 6,    1, 6, 5,
        // top triangles
        3, 7, 6,    3, 6, 2,
        // bottom triangles
        0, 1, 5,    0, 5, 4
    };

    // define and upload element buffer data to the GPU
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLsizei indexCount = sizeof(indices) / sizeof(indices[0]);
    GLint success;

    // create vertex shader & check compilation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader compilation failure\n" << infoLog << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // create fragment shader & check compilation
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader compilation failure\n" << infoLog << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // create shader program to link shaders & check linking success
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program linking failure\n" << infoLog << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // remove shaders and set program state
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

    GLint timeLocation = glGetUniformLocation(shaderProgram, "time");

    // define transformation matrices for the object
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 transform = glm::translate(identity, glm::vec3(0.0f, 0.0f, 0.0f)) * 
                          glm::rotate(identity, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f)) *
                          glm::scale(identity, glm::vec3(1.2f, 1.2f, 1.2f));

    float yaw = -90.0f;
    float pitch = 0.0f;

    // camera setup
    Camera camera = Camera();
    glfwSetWindowUserPointer(window, &camera);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
    // define model-view-projection matrix
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * transform; 
    GLint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");

    // initialize timing variables for frame updates
    float currentFrame, deltaTime, lastFrame = 0.0f;

    float lastMouseX = 400.0f;
    float lastMouseY = 300.0f;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // update timing variables for the current frame
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // read user-input and update camera position
        camera.cameraMovement(window, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // update view and mvp matrices based on the new camera position
        view = camera.getViewMatrix();
        mvp = projection * view * transform;

        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set shader uniforms and draw the object
        glUniform1f(timeLocation, static_cast<float>(glfwGetTime()));
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
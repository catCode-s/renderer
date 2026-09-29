#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// define source code for vertex shader (C)
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

// define source code for fragment shader (C)
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    // make the target to render to this window
    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cerr << "Could not retrieve OpenGL function addresses\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

    // CPU-side vertex data
    float vertices[] = {
        // verticies         // colors
       -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    // specifies data itself
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // specifies how data should be interpreted
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLsizei indexCount = sizeof(indices) / sizeof(indices[0]);

    // create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // check shader compilation
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader compilation failure\n" << infoLog << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader compilation failure\n" << infoLog << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // create shader program to link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check link success
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
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 transform = glm::translate(identity, glm::vec3(0.3f, 0.0f, 0.0f)) * 
                          glm::rotate(identity, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(identity, glm::vec3(0.5f, 0.5f, 1.0f));
    // GLint transformLocation = glGetUniformLocation(shaderProgram, "transform");

    glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * transform;
    GLint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniform1f(timeLocation, static_cast<float>(glfwGetTime()));
        // glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
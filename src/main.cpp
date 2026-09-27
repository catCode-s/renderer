#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// define source code for vertex shader (C)
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(position, 1.0);\n"
    "}\n";

// define source code for fragment shader (C)
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 fragmentColor;\n"
    "void main()\n"
    "{\n"
    "    fragmentColor = vec4(0.2, 0.6, 1.0, 1.0);\n"
    "}\n";

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

    glClearColor(0.9f, 0.5f, 0.2f, 1.0f);

    // CPU-side vertex data
    float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

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

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
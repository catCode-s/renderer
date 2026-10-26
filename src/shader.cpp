#include "shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

/*
 * read the contents of a file and return it as a string
 * @param path path to the file to read
 * @throws std::runtime_error if the file cannot be opened
 * @return contents of the file as a string, throws std::runtime_error if the file cannot be opened
 */
std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error("Failed to open shader: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

/**
 * compile a shader from source code and return its ID
 * @param type type of shader to compile (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 * @param source source code of the shader
 * @throws std::runtime_error if the shader cannot be compiled
 * @return ID of the compiled shader, throws std::runtime_error if the shader cannot be compiled
 */
GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);

    const char* sourcePointer = source.c_str();
    glShaderSource(shader, 1, &sourcePointer, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);

        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed:\n" + std::string(infoLog));
    }

    return shader;
}

/**
 * constructor for the shader class,
 * creates a shader program from vertex and fragment shader source files
 * @param vertexPath path to the vertex shader source file
 * @param fragmentPath path to the fragment shader source file
 */
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);

        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

/**
 * destructor for shader class
 */
Shader::~Shader() { glDeleteProgram(program); }

/**
 * use the shader program for rendering
 */
void Shader::use() const { glUseProgram(program); }

/**
 * set an int uniform value for the shader program
 */
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

/**
 * set a float uniform value for the shader program
 */
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

/**
 * set a 4x4 matrix uniform value for the shader program
 */
void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * set a 3D vector uniform value for the shader program
 * @param name name of the uniform variable
 * @param vector value to set for the uniform variable
 */
void Shader::setVec3(const std::string& name, const glm::vec3& vector) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(vector));
}
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

/**
 * A class that represents a shader program in OpenGL
 * compiles and links vertex and fragment shaders,
 * and sets uniform values for the shader program
 */
class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    GLuint program;

    static std::string readFile(const std::string& path);
    static GLuint compileShader(GLenum type, const std::string& source);
};
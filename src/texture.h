#pragma once

#include <glad/glad.h>
#include <string>

/**
 * A class that represents a texture in OpenGL
 */
class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    void bind(GLuint unit = 0) const;

private:
    GLuint texture;
};
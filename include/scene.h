#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "mesh.h"

/**
 * Represents the transformation of an object in the scene.
 */
struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    glm::mat4 getModelMatrix() const;
};

/**
 * Represents an object in the scene, including its mesh and transformation.
 */
class SceneObject {
public:
    explicit SceneObject(Mesh& mesh);

    Transform& getTransform();
    const Transform& getTransform() const;

    const Mesh& getMesh() const;

private:
    Mesh* mesh;
    Transform transform;
};

/**
 * Represents a point light source in the scene.
 */
struct PointLight {
    glm::vec3 position{0.0f};
    glm::vec3 color{1.0f};
    float intensity{1.0f};
};

/**
 * Represents the scene, including all objects and lights.
 */
class Scene {
public:
    SceneObject& addObject(Mesh& mesh);

    std::vector<SceneObject>& getObjects();
    const std::vector<SceneObject>& getObjects() const;

private:
    std::vector<SceneObject> objects;
};
#include "scene.h"

/**
 * constructor for SceneObject
 * @param mesh the mesh to be used by the scene object
 */
SceneObject::SceneObject(Mesh& mesh) : mesh(&mesh) {}

/**
 * add a new scene object to the scene
 * @param mesh the mesh to be used by the scene object
 * @return a reference to the newly added scene object
 */
SceneObject& Scene::addObject(Mesh& mesh) {
    objects.emplace_back(mesh);
    return objects.back();
}

/**
 * get the list of scene objects in the scene
 * @return a reference to the vector of scene objects
 */
std::vector<SceneObject>& Scene::getObjects() {
    return objects;
}

/**
 * get the list of scene objects in the scene (const version)
 * @return a const reference to the vector of scene objects
 */
const std::vector<SceneObject>& Scene::getObjects() const {
    return objects;
}

/**
 * get the transform of the scene object
 * @return a reference to the transform
 */
Transform& SceneObject::getTransform() {
    return transform;
}

/**
 * get the transform of the scene object (const version)
 * @return a const reference to the transform
 */
const Transform& SceneObject::getTransform() const {
    return transform;
}

/**
 * get the mesh of the scene object (const version)
 * @return a const reference to the mesh
 */
const Mesh& SceneObject::getMesh() const {
    return *mesh;
}

/**
 * get the model matrix of the transform
 * @return the model matrix
 */
glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}
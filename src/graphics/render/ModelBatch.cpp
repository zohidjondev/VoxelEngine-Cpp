#include "ModelBatch.hpp"

#include "../core/Mesh.hpp"
#include "../core/Texture.hpp"
#include "../../window/Window.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

/// xyz, uv, compressed rgba
inline constexpr uint VERTEX_SIZE = 6;

static const vattr attrs[] = {
    {3}, {2}, {1}, {0}
};

static glm::vec3 SUN_VECTOR (0.411934f, 0.863868f, -0.279161f);
inline constexpr glm::vec3 X(1, 0, 0);
inline constexpr glm::vec3 Y(0, 1, 0);
inline constexpr glm::vec3 Z(0, 0, 1);

ModelBatch::ModelBatch(size_t capacity) 
  : buffer(std::make_unique<float[]>(capacity * VERTEX_SIZE)),
    capacity(capacity),
    index(0),
    mesh(std::make_unique<Mesh>(buffer.get(), 0, attrs)),
    combined(1.0f)
{
    ubyte pixels[] = {
        255, 255, 255, 255,
    };
    blank = std::make_unique<Texture>(pixels, 1, 1, ImageFormat::rgba8888);
}

ModelBatch::~ModelBatch() {
}

void ModelBatch::test(glm::vec3 pos, glm::vec3 size) {
    float time = static_cast<float>(Window::time());
    pushMatrix(glm::translate(glm::mat4(1.0f), pos));
    pushMatrix(glm::rotate(glm::mat4(1.0f), time, glm::vec3(0,1,0)));
    pushMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, glm::sin(time*2), 0)));
    box({}, size);
    popMatrix();
    popMatrix();
    popMatrix();
}

void ModelBatch::box(glm::vec3 pos, glm::vec3 size) {
    plane(pos+Z, X*size, Y*size, Z);
    plane(pos-Z, -X*size, Y*size, -Z);

    plane(pos+Y, X*size, -Z*size, Y);
    plane(pos-Y, X*size, Z*size, -Y);

    plane(pos+X, -Z*size, Y*size, X);
    plane(pos-X, Z*size, Y*size, -X);
}

void ModelBatch::flush() {
    if (index == 0) {
        return;
    }
    blank->bind();
    mesh->reload(buffer.get(), index / VERTEX_SIZE);
    mesh->draw();
    index = 0;
}

void ModelBatch::pushMatrix(glm::mat4 matrix) {
    matrices.push_back(combined);
    combined = matrix * combined;

    decomposed = {};
    glm::quat rotation;
    glm::decompose(
        combined, 
        decomposed.scale, 
        rotation, 
        decomposed.translation, 
        decomposed.skew, 
        decomposed.perspective
    );
    decomposed.rotation = glm::toMat3(rotation);
}

void ModelBatch::popMatrix() {
    combined = matrices[matrices.size()-1];
    matrices.erase(matrices.end()-1);

    decomposed = {};
    glm::quat rotation;
    glm::decompose(
        combined, 
        decomposed.scale, 
        rotation, 
        decomposed.translation, 
        decomposed.skew, 
        decomposed.perspective
    );
    decomposed.rotation = glm::toMat3(rotation);
}

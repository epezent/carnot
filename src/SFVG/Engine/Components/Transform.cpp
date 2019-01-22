#include <SFVG/Engine/Components/Transform.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Math.hpp>

namespace sfvg {

//==============================================================================
// Constructor
//==============================================================================

Transform::Transform(GameObject& _gameObject) :
    Component(_gameObject),
    m_origin(0, 0),
    m_position(0, 0),
    m_rotation(0),
    m_scale(1, 1),
    m_transform(),
    m_globalTransform(),
    m_inverseTransform(),
    m_inverseGlobalTransform(),
    m_transformNeedUpdate(true),
    m_globalTransformNeedUpdate(true),
    m_inverseTransformNeedUpdate(true),
    m_invGlobTransformNeedUpdate(true)
{

}

//==============================================================================
// Local Transformatioins
//==============================================================================

void Transform::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Transform::setPosition(const Vector2f& position) {
    setPosition(position.x, position.y);
}

void Transform::setRotation(float angle) {
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Transform::setScale(float factorX, float factorY) {
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Transform::setScale(const Vector2f& factors) {
    setScale(factors.x, factors.y);
}

void Transform::setOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Transform::setOrigin(const Vector2f& origin) {
    setOrigin(origin.x, origin.y);
}

const Vector2f& Transform::getPosition() const {
    return m_position;
}

float Transform::getRotation() const {
    return m_rotation;
}

const Vector2f& Transform::getScale() const {
    return m_scale;
}

const Vector2f& Transform::getOrigin() const {
    return m_origin;
}

void Transform::move(float offsetX, float offsetY) {
    setPosition(m_position.x + offsetX, m_position.y + offsetY);
}


void Transform::move(const Vector2f& offset) {
    setPosition(m_position.x + offset.x, m_position.y + offset.y);
}

void Transform::rotate(float angle) {
    setRotation(m_rotation + angle);}


void Transform::scale(float factorX, float factorY) {
    setScale(m_scale.x * factorX, m_scale.y * factorY);
}

void Transform::scale(const Vector2f& factor) {
    setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}

const Matrix3x3& Transform::getLocalMatrix() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle = -m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine = static_cast<float>(std::sin(angle));
        float sxc = m_scale.x * cosine;
        float syc = m_scale.y * cosine;
        float sxs = m_scale.x * sine;
        float sys = m_scale.y * sine;
        float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

        m_transform = Matrix3x3(sxc, sys, tx,
                               -sxs, syc, ty,
                                0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}

const Matrix3x3& Transform::getInverseLocalMatrix() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = getLocalMatrix().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

//==============================================================================
// Global Transformatioins
//==============================================================================

Matrix3x3 Transform::getGlobalMatrix() const {
    Matrix3x3 matrix = Matrix3x3::Identity;
    for (const GameObject* node = &gameObject; node != nullptr; node = node->m_parent)
        matrix = node->transform.getLocalMatrix() * matrix;
    return matrix;
}

Matrix3x3 Transform::getInverseGlobalMatrix() const {
    return getGlobalMatrix().getInverse();
}

void Transform::setGlobalPosition(const Vector2f& position) {
    setPosition(getInverseGlobalMatrix().transformPoint(position));
}
void Transform::setGlobalPosition(float x, float y) {
    setGlobalPosition(Vector2f(x, y));
}

Vector2f Transform::getGlobalPosition() const {
    return getGlobalMatrix() * Vector2f();
}

void Transform::setGlobalRotation(float angle) {
    rotate(angle - getGlobalRotation());
}

float Transform::getGlobalRotation() const {
    auto m = getGlobalMatrix().getMatrix();
    float angle = std::atan2(m[1] , m[5]) * sfvg::RAD2DEG;
    angle = static_cast<float>(fmod(angle, 360));
    if (angle < 0)
        angle += 360.f;
    return angle;
}

void Transform::setGlobalScale(float factorX, float factorY) {
    setGlobalScale(Vector2f(factorX, factorY));
}


void Transform::setGlobalScale(const Vector2f& factors) {
    auto scaleBy = getGlobalScale();
    scaleBy.x = factors.x / scaleBy.x;
    scaleBy.y = factors.y / scaleBy.y;
    scale(scaleBy);
}

const Vector2f Transform::getGlobalScale() const {
    auto m = getGlobalMatrix().getMatrix();
    Vector2f scale;
    scale.x = std::sqrt(m[0] * m[0] + m[1] * m[1]);
    scale.y = std::sqrt(m[4] * m[4] + m[5] * m[5]);
    return scale;
}


} // namespace sfvg

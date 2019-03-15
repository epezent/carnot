#include <SFVG/Engine/Components/Transform.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Common/Math.hpp>
#include <SFVG/Engine/ImGui/imgui.h>

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
    m_localTransform(),
    m_worldTransform(),
    m_invLocalTransform(),
    m_invWorldTransform(),
    m_localTransformDirty(true),
    m_worldTransformDirty(true),
    m_invLocalTransformDirty(true),
    m_invWorldTransformDirty(true)
{

}

//==============================================================================
// Local Transformatioins
//==============================================================================

void Transform::setLocalPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    makeDirty();
}

void Transform::setLocalPosition(const Vector2f& position) {
    setLocalPosition(position.x, position.y);
}

void Transform::setLocalRotation(float angle) {
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;
    makeDirty();
}

void Transform::setLocalScale(float factorX, float factorY) {
    m_scale.x = factorX;
    m_scale.y = factorY;
    makeDirty();
}

void Transform::setLocalScale(const Vector2f& factors) {
    setLocalScale(factors.x, factors.y);
}

void Transform::setLocalOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
    makeDirty();
}

void Transform::setLocalOrigin(const Vector2f& origin) {
    setLocalOrigin(origin.x, origin.y);
}

const Vector2f& Transform::getLocalPosition() const {
    return m_position;
}

float Transform::getLocalRotation() const {
    return m_rotation;
}

const Vector2f& Transform::getLocalScale() const {
    return m_scale;
}

const Vector2f& Transform::getLocalOrigin() const {
    return m_origin;
}

void Transform::move(float offsetX, float offsetY) {
    setLocalPosition(m_position.x + offsetX, m_position.y + offsetY);
}


void Transform::move(const Vector2f& offset) {
    setLocalPosition(m_position.x + offset.x, m_position.y + offset.y);
}

void Transform::rotate(float angle) {
    setLocalRotation(m_rotation + angle);}


void Transform::scale(float factorX, float factorY) {
    setLocalScale(m_scale.x * factorX, m_scale.y * factorY);
}

void Transform::scale(const Vector2f& factor) {
    setLocalScale(m_scale.x * factor.x, m_scale.y * factor.y);
}

const Matrix3x3& Transform::getLocalMatrix() const
{
    // Recompute the combined transform if needed
    if (m_localTransformDirty)
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

        m_localTransform = Matrix3x3(sxc, sys, tx,
                               -sxs, syc, ty,
                                0.f, 0.f, 1.f);
        m_localTransformDirty = false;
    }

    return m_localTransform;
}

const Matrix3x3& Transform::getInverseLocalMatrix() const {
    // Recompute the inverse transform if needed
    if (m_invLocalTransformDirty) {
        m_invLocalTransform = getLocalMatrix().getInverse();
        m_invLocalTransformDirty = false;
    }
    return m_invLocalTransform;
}

//==============================================================================
// Global Transformatioins
//==============================================================================

Matrix3x3 Transform::getWorldMatrix() const {
    Matrix3x3 matrix = Matrix3x3::Identity;
    for (const GameObject* node = &gameObject; node != nullptr; node = node->m_parent)
        matrix = node->transform.getLocalMatrix() * matrix;
    return matrix;
}

Matrix3x3 Transform::getInverseWorldMatrix() const {
    return getWorldMatrix().getInverse();
}

void Transform::setPosition(const Vector2f& position) {
    auto parentMatrix = getInverseWorldMatrix() * getLocalMatrix();
    setLocalPosition(parentMatrix.transformPoint(position));
}

void Transform::setPosition(float x, float y) {
    setPosition(Vector2f(x, y));
}

Vector2f Transform::getPosition() const {
    return getWorldMatrix() * getLocalOrigin();
}

void Transform::setRotation(float angle) {
    rotate(angle - getRotation());
}

float Transform::getRotation() const {
    auto m = getWorldMatrix().getMatrix();
    float angle = std::atan2(m[1] , m[5]) * sfvg::RAD2DEG;
    angle = static_cast<float>(fmod(angle, 360));
    if (angle < 0)
        angle += 360.f;
    return angle;
}

void Transform::setScale(float factorX, float factorY) {
    setScale(Vector2f(factorX, factorY));
}


void Transform::setScale(const Vector2f& factors) {
    auto scaleBy = getScale();
    scaleBy.x = factors.x / scaleBy.x;
    scaleBy.y = factors.y / scaleBy.y;
    scale(scaleBy);
}

const Vector2f Transform::getScale() const {
    auto m = getWorldMatrix().getMatrix();
    Vector2f scale;
    scale.x = std::sqrt(m[0] * m[0] + m[1] * m[1]);
    scale.y = std::sqrt(m[4] * m[4] + m[5] * m[5]);
    return scale;
}

//==========================================================================
// Utility Functions
//==========================================================================

// SFML Transform.getMatrix()
//     m_localTransform.getMatrix();
//     [m0  m4  m8  m12]    []
//     [m1  m5  m9  m13]
//     [m2  m6  m10 m14]
//     [m3  m4  m11 m15]

Vector2f Transform::localToWorld(const Vector2f& point) {
    return getWorldMatrix().transformPoint(point);
}

FloatRect Transform::localToWorld(const FloatRect& rect) {
    return getWorldMatrix().transformRect(rect);
}

Vector2f Transform::worldToLocal(const Vector2f& point) {
    return getInverseWorldMatrix().transformPoint(point);
}

FloatRect Transform::worldToLocal(const FloatRect& rect) {
    return getInverseWorldMatrix().transformRect(rect);

    m_localTransform.getMatrix();
}

//==============================================================================
// PRIVATE
//==============================================================================

void Transform::onGizmo() {
    static Id transformId = Debug::gizmoId("Transform");
    if (Debug::gizmoActive(transformId)) {
        auto x = localToWorld(Vector2f(20.0f,0.0f) + getLocalOrigin());
        auto y = localToWorld(Vector2f(0.0f,20.0f) + getLocalOrigin());

        if (magnitude(Input::getMousePosition() - getPosition()) < 20.0f)
            Debug::drawText(gameObject.getName(), Input::getMousePosition() - Vector2f(5,5), Debug::gizmoColor(transformId));

        Debug::drawPoint(getPosition(), Debug::gizmoColor(transformId));
        Debug::drawPoint(x, Reds::FireBrick);
        Debug::drawPoint(y, Greens::LightGreen);
        Debug::drawLine(getPosition(), x, Reds::FireBrick);
        Debug::drawLine(getPosition(), y, Greens::LightGreen);
    }
}

void Transform::makeDirty() {
    m_localTransformDirty = true;
    m_invLocalTransformDirty = true;
    m_worldTransformDirty = true;
    m_invWorldTransformDirty = true;
    callCallbacks();
}

//==============================================================================
// Callbacks
//==============================================================================

void Transform::registerCallback(std::function<void(void)> callback) {
    m_callbacks.push_back(callback);
}

void Transform::callCallbacks() {
    // call our callbacks
    for (auto& c : m_callbacks)
        c();
    // call children callbacks
    for (std::size_t i = 0; i < gameObject.getChildCount(); ++i)
        gameObject.getChild(i)->transform.callCallbacks();
}

} // namespace sfvg


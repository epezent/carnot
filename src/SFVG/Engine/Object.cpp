#pragma warning (disable : 4100)

#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Coroutine.hpp>
#include <SFVG/Graphics.hpp>
#include <algorithm>
#include <cmath>

namespace sfvg {

//==============================================================================
// Constructor/Destructor
//==============================================================================

static std::size_t g_nameIndex = 0;
static std::size_t g_objectCount = 0;

Object::Object() :
    m_enabled(true),
    m_layer(0),
    m_children(),
    m_parent(nullptr),
    m_index(0),
    m_isRoot(false),
    m_iteratingChildren(false),
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
    m_id = ID::makeId("obj" + std::to_string(g_nameIndex));
    g_nameIndex++;
    g_objectCount++;
}

Object::Object(const Name& name) :
    m_enabled(true),
    m_layer(0),
    m_children(),
    m_parent(nullptr),
    m_index(0),
    m_isRoot(false),
    m_iteratingChildren(false),
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
    m_id = ID::makeId(name);
    g_objectCount++;
}

Object::~Object() {
    stopAllCoroutines();
    g_objectCount--;
    ID::freeId(m_id);
}

//==============================================================================
// General
//==============================================================================

void Object::setName(const Name& name) {
    ID::freeId(m_id);
    m_id = ID::makeId(name);
    onRenamed(name);
}

const Name& Object::getName() const {
    return ID::getName(m_id);
}

Id Object::getId() const {
    return m_id;
}

Handle<Object> Object::getHandle() const {
    if (isRoot())
        return m_engine->getRoot();
    assert(hasParent());
    return m_parent->getChild(getIndex());
}

void Object::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Object::isEnabled() const {
    return m_enabled;
}

void Object::destroy() {
    assert(hasParent());
    m_parent->destroyChild(m_index);
}

bool Object::isRoot() const {
    return m_isRoot;
}

//==============================================================================
// Layer Functions
//==============================================================================

void Object::setLayer(std::size_t layer) {
    assert(layer < m_engine->getLayerCount());
    m_layer = layer;
}

std::size_t Object::getLayer() const {
    return m_layer;
}

void Object::incrementLayer() {
    if (m_layer < (m_engine->getLayerCount() - 1))
        m_layer++;
}

void Object::decrementLayer() {
    if (m_layer > 0)
        m_layer--;
}

void Object::sendToBack() {
    m_layer = 0;
}

void Object::sendToFront() {
    m_layer = m_engine->getLayerCount() - 1;
}

//==============================================================================
// Local Transformatioins
//==============================================================================

void Object::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Object::setPosition(const Vector2f& position) {
    setPosition(position.x, position.y);
}

void Object::setRotation(float angle) {
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Object::setScale(float factorX, float factorY) {
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Object::setScale(const Vector2f& factors) {
    setScale(factors.x, factors.y);
}

void Object::setOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
    m_globalTransformNeedUpdate = true;
    m_invGlobTransformNeedUpdate = true;
}

void Object::setOrigin(const Vector2f& origin) {
    setOrigin(origin.x, origin.y);
}

const Vector2f& Object::getPosition() const {
    return m_position;
}

float Object::getRotation() const {
    return m_rotation;
}

const Vector2f& Object::getScale() const {
    return m_scale;
}

const Vector2f& Object::getOrigin() const {
    return m_origin;
}

void Object::move(float offsetX, float offsetY) {
    setPosition(m_position.x + offsetX, m_position.y + offsetY);
}


void Object::move(const Vector2f& offset) {
    setPosition(m_position.x + offset.x, m_position.y + offset.y);
}

void Object::rotate(float angle) {
    setRotation(m_rotation + angle);}


void Object::scale(float factorX, float factorY) {
    setScale(m_scale.x * factorX, m_scale.y * factorY);
}

void Object::scale(const Vector2f& factor) {
    setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}

const Transform& Object::getTransform() const
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

        m_transform = Transform(sxc, sys, tx,
                               -sxs, syc, ty,
                                0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}

const Transform& Object::getInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = getTransform().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

//==============================================================================
// Global Transformatioins
//==============================================================================

Transform Object::getGlobalTransform() const {
    Transform transform = Transform::Identity;
    for (const Object* node = this; node != nullptr; node = node->m_parent)
        transform = node->getTransform() * transform;
    return transform;
}

Transform Object::getInverseGlobalTransform() const {
    return getGlobalTransform().getInverse();
}

void Object::setGlobalPosition(const Vector2f& position) {
    setPosition(getInverseGlobalTransform().transformPoint(position));
}
void Object::setGlobalPosition(float x, float y) {
    setGlobalPosition(Vector2f(x, y));
}

Vector2f Object::getGlobalPosition() const {
    return getGlobalTransform() * Vector2f();
}

void Object::setGlobalRotation(float angle) {
    rotate(angle - getGlobalRotation());
}

float Object::getGlobalRotation() const {
    auto m = getGlobalTransform().getMatrix();
    float angle = std::atan2(m[1] , m[5]) * sfvg::RAD2DEG;
    angle = static_cast<float>(fmod(angle, 360));
    if (angle < 0)
        angle += 360.f;
    return angle;
}

void Object::setGlobalScale(float factorX, float factorY) {
    setGlobalScale(Vector2f(factorX, factorY));
}


void Object::setGlobalScale(const Vector2f& factors) {
    auto scaleBy = getGlobalScale();
    scaleBy.x = factors.x / scaleBy.x;
    scaleBy.y = factors.y / scaleBy.y;
    scale(scaleBy);
}

const Vector2f Object::getGlobalScale() const {
    auto m = getGlobalTransform().getMatrix();
    Vector2f scale;
    scale.x = std::sqrt(m[0] * m[0] + m[1] * m[1]);
    scale.y = std::sqrt(m[4] * m[4] + m[5] * m[5]);
    return scale;
}

//==============================================================================
// Children
//==============================================================================

std::size_t Object::getChildCount() const {
    return m_children.size();
}

void Object::attachChild(Ptr<Object> gameObject) {
    gameObject->m_parent = this;
    gameObject->setEngine(m_engine);
    if (!m_iteratingChildren) {
        m_children.push_back(std::move(gameObject));
        updateChildIndices();
    }
    else {
        m_additions.push_back(std::move(gameObject));
    }
}

Ptr<Object> Object::detachChild(std::size_t index) {
    assert(index < m_children.size());
    if (!m_iteratingChildren) {
        // children are not being iterated, so detach child now
        Ptr<Object> obj = std::move(m_children[index]);
        m_children.erase(m_children.begin() + index);
        obj->m_parent = nullptr;
        obj->setEngine(nullptr);
        updateChildIndices();
        return obj;
    }
    else {
        // children are being iterated, so defer removal until later
        m_children[index]->m_parent = nullptr;
        m_children[index]->setEngine(nullptr);
        destroyChild(index);
        return m_children[index];
    }
}

void Object::destroyChild(std::size_t index) {
    assert(index < m_children.size());
    m_deletions.push_back(m_children[index].get());
}

void Object::destroyChildren() {
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        m_deletions.push_back(m_children[i].get());
    }
}

Handle<Object> Object::getChild(std::size_t index) {
    assert(index < m_children.size());
    return Handle<Object>(m_children[index]);
}

void Object::makeChildFirst(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<Object> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.insert(m_children.begin(), std::move(obj));
    updateChildIndices();
}

void Object::makeChildLast(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<Object> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.push_back(std::move(obj));
    updateChildIndices();
}

Handle<Object> Object::findChild(Id id) {
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        if (m_children[i]->m_id == id)
            return Handle<Object>(m_children[i]);
    }
    return Handle<Object>();
}

Handle<Object> Object::findChild(const Name& name) {
    Id id = ID::getId(name);
    return findChild(id);
}

bool Object::isParentOf(Handle<Object> object) {
    return object->m_parent == this;
}

//==============================================================================
// Parent
//==============================================================================

bool Object::hasParent() const {
    return m_parent != nullptr;
}

Handle<Object> Object::getParent() const {
    assert(hasParent());
    return m_parent->getHandle();
}

std::size_t Object::getIndex() const {
    return m_index;
}

bool Object::isChildOf(Handle<Object> object) {
    return m_parent == object.get();
}


//==============================================================================
// Public Static Functions
//==============================================================================

std::size_t Object::getObjectCount() {
    return g_objectCount;
}

//==============================================================================
// Protected Functions
//==============================================================================

Engine& Object::getEngine() const {
    return *m_engine;
}

//==============================================================================
// Virtual Functions
//==============================================================================

void Object::update() {
    // do nothing by default
}

void Object::draw(RenderTarget& target, RenderStates states) const {
    // do nothing by default
}

void Object::onRenamed(const Name& newName) {
    // do nothing by default
}

//==============================================================================
// Private Functions
//==============================================================================

void Object::setEngine(Engine* engine) {
    m_engine = engine;
    for (const auto& child : m_children)
        child->setEngine(m_engine);
}

void Object::updateChildIndices() {
    for (std::size_t i = 0; i < m_children.size(); ++i)
        m_children[i]->m_index = i;
}

void Object::updateAll() {
    // add Objects in additions
    if (m_additions.size() > 0) {
        for (std::size_t i = 0; i < m_additions.size(); ++i) {
            attachChild(std::move(m_additions[i]));
        }
        m_additions.clear();
    }
    if (m_enabled) {
        // update this
        update();
        // resume coroutines
        if (hasCoroutines())
            resumeCoroutines();
        // update children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->updateAll();
        m_iteratingChildren = false;
    }
    // destroy Objects in deletions
    if (m_deletions.size() > 0) {
        for (std::size_t i = 0; i < m_deletions.size(); ++i) {
            detachChild(m_deletions[i]->getIndex());
        }
        m_deletions.clear();
    }
}

void Object::queRender(RenderQue& renderQue, RenderStates states) const {
    if (m_enabled) {
        // set states
        states.transform *= getTransform();
        // que this Object
        renderQue[m_layer].push_back({ this, states });
        // que children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->queRender(renderQue, states);
        m_iteratingChildren = false;
    }
}

//=============================================================================
// Coroutines
//=============================================================================


Handle<Coroutine> Object::startCoroutine(Enumerator&& e) {
    auto h = e.getCoroutine();
    m_coroutines.push_back(std::move(e));
    return h;
}

void Object::resumeCoroutines() {
    std::vector<Enumerator> coroutinesCopy;
    coroutinesCopy.swap(m_coroutines);
    for (auto& coro : coroutinesCopy) {
        auto curr = coro.currentValue();
        if (curr) { // current yield instruction is valid and not nullptr
            if (curr->isOver()) { // current yield instruction is done
                // resume coroutine
                if (coro.next()) // coroutine isn't done
                    // reque coroutine
                    m_coroutines.push_back(std::move(coro));
            }
            else {
                // reque coroutine
                m_coroutines.push_back(std::move(coro));
            }
        }
        else { // current yield instruction is nullptr
            if (coro.next()) {
                // reque coroutine
                m_coroutines.push_back(std::move(coro));
            }
        }
    }
}

void Object::stopAllCoroutines() {
    // delete all coroutines
    m_coroutines.clear();
}

bool Object::hasCoroutines() const {
    return m_coroutines.size() > 0;
}

std::size_t Object::getCoroutineCount() const {
    return m_coroutines.size();
}

} // namespace sfvg

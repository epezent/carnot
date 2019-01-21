#include <SFVG/Engine/Systems/InputSystem.hpp>
#include <SFVG/Print.hpp>
#include <SFVG/Engine/Engine.hpp>

namespace sfvg {

InputSystem::InputSystem(Engine& engine, const Name& name) :
    System(engine, name),
    m_mouseScrollDetla(0.0f),
    m_dblClkDelay(0.25f),
    m_anyKeyDownBool(false),
    m_anyKeyUpBool(false),
    m_anyMouseDownBool(false),
    m_anyMouseUpBool(false),
    m_anyInputBool(false),
    m_textEntered(""),
    m_keyDownTable({false})
{
    m_keyDownTable.fill(false);
    m_keyUpTable.fill(false);
    m_mouseDownTable.fill(false);
    m_mouseUpTable.fill(false);
    m_dblClkTable.fill(false);
    m_dblClkTimes.fill(0.0f);
    m_dragStartedTable.fill(false);
    m_dragEndedTable.fill(false);
    m_draggingTable.fill(false);
    m_dragStartTable.fill(Vector2f());
    m_dragLastTable.fill(Vector2f());
    m_dragDeltaTable.fill(Vector2f());
    m_dragTotalTable.fill(Vector2f());
}

void InputSystem::processEvent(const Event& event) {
    switch (event.type) {
        case Event::KeyPressed: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                m_keyDownTable[code] = true;
            m_anyInputBool = true;
            m_anyKeyDownBool = true;
            break;
        }
        case Event::KeyReleased: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                m_keyUpTable[code]   = true;
            m_anyKeyUpBool = true;
            break;
        }
        case Event::MouseButtonPressed: {
            m_mouseDownTable[event.mouseButton.button] = true;
            m_anyInputBool = true;
            m_anyMouseDownBool = true;
            // drag
            m_dragStartedTable[event.mouseButton.button] = true;
            m_draggingTable[event.mouseButton.button] = true;
            m_dragStartTable[event.mouseButton.button] = m_worldPosition;
            m_dragLastTable[event.mouseButton.button]  = m_worldPosition;
            // double click
            float now = engine.time();
            if ((now - m_dblClkTimes[event.mouseButton.button]) < m_dblClkDelay) {
                m_dblClkTable[event.mouseButton.button] = true;
                m_dragStartedTable[event.mouseButton.button] = false;
                m_draggingTable[event.mouseButton.button] = false;
            }
            m_dblClkTimes[event.mouseButton.button] = now;
            break;
        }
        case Event::MouseButtonReleased: {
            m_mouseUpTable[event.mouseButton.button] = true;
            m_anyMouseUpBool = true;
            // drag
            m_dragEndedTable[event.mouseButton.button] = true;
            m_draggingTable[event.mouseButton.button]  = false;
            break;
        }
        case Event::MouseWheelScrolled: {
            m_mouseScrollDetla += event.mouseWheelScroll.delta;
            m_anyInputBool = true;
            break;
        }
        case Event::TextEntered: {
            if (event.text.unicode > 31 && event.text.unicode < 128)
                m_textEntered += event.text.unicode;
            break;
        }
        default:
            break;
    }
}

void InputSystem::clearState() {
    for (std::size_t i = 0; i < Key::KeyCount; ++i) {
        m_keyDownTable[i] = false;
        m_keyUpTable[i] = false;
    }
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        m_mouseDownTable[i] = false;
        m_mouseUpTable[i] = false;
        m_dblClkTable[i] = false;
        m_dragStartedTable[i] = false;
        m_dragEndedTable[i] = false;
        m_dragDeltaTable[i] = Vector2f();
    }
    m_mouseScrollDetla = 0;
    m_textEntered = "";
    m_anyInputBool = false;
    m_anyKeyDownBool = false;
    m_anyKeyUpBool = false;
    m_anyMouseDownBool = false;
    m_anyMouseUpBool = false;
}

void InputSystem::update() {
    clearState();
    m_mousePosition = sf::Mouse::getPosition(engine.window);
    m_worldPosition = engine.window.mapPixelToCoords(m_mousePosition, engine.getView(0));
    // update draggings
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        if (m_draggingTable[i]) {
            m_dragDeltaTable[i] = m_worldPosition - m_dragLastTable[i];
            m_dragTotalTable[i] = m_worldPosition - m_dragStartTable[i];
            m_dragLastTable[i]  = m_worldPosition;
        }
    }
}

//==============================================================================
// PUBLIC STATIC FUNCTIONS
//==============================================================================

bool InputSystem::getKey(Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool InputSystem::getKeyDown(Key key) {
    return m_keyDownTable[key];
}

bool InputSystem::getKeyUp(Key key) {
    return m_keyUpTable[key];
}

bool InputSystem::getKeyDown() {
    return m_anyKeyDownBool;
}

bool InputSystem::getKeyUp() {
    return m_anyKeyUpBool;
}

bool InputSystem::getMouse(MouseButton button) {
    return sf::Mouse::isButtonPressed(button);
}

bool InputSystem::getMouseDown(MouseButton button) {
    return m_mouseDownTable[button];
}

bool InputSystem::getMouseUp(MouseButton button) {
    return m_mouseUpTable[button];
}

bool InputSystem::getMouseDown() {
    return m_anyMouseDownBool;
}

bool InputSystem::getMouseUp() {
    return m_anyMouseUpBool;
}

bool InputSystem::getDoubleClick(MouseButton button) {
    return m_dblClkTable[button];
}

bool InputSystem::dragStarted(MouseButton button) {
    return m_dragStartedTable[button];
}

bool InputSystem::dragging(MouseButton button) {
    return m_draggingTable[button];
}

bool InputSystem::dragEnded(MouseButton button) {
    return m_dragEndedTable[button];
}

Vector2f InputSystem::dragDelta(MouseButton button) {
    return m_dragDeltaTable[button];
}

float InputSystem::getScroll() {
    return m_mouseScrollDetla;
}

Vector2i InputSystem::getRawMousePosition() {
    return m_mousePosition;
}

Vector2f InputSystem::getMousePosition() {
    return m_worldPosition;
}

Vector2f InputSystem::getMousePosition(const View& view) {
    return engine.window.mapPixelToCoords(m_mousePosition, view);
}

const sf::String& InputSystem::getTextEntered() {
    return m_textEntered;
}

bool InputSystem::anyInput() {
    return m_anyInputBool;
}

} // namespace sfvg

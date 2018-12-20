#include <SFVG/Engine/Input.hpp>
#include <SFVG/Engine/Print.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <array>

//==============================================================================
// STATIC VARIABLES
//==============================================================================

namespace {

    static std::array<bool, Key::KeyCount>                    keyDownTable;
    static std::array<bool, Key::KeyCount>                    keyUpTable;
    static std::array<bool, MouseButton::ButtonCount>         mouseDownTable;
    static std::array<bool, MouseButton::ButtonCount>         mouseUpTable;
    static std::array<bool, MouseButton::ButtonCount>         dblClkTable;
    static std::array<float, MouseButton::ButtonCount>        dblClkTimes;
    static std::array<bool, MouseButton::ButtonCount>         dragStartedTable;
    static std::array<bool, MouseButton::ButtonCount>         dragEndedTable;
    static std::array<bool, MouseButton::ButtonCount>         draggingTable;
    static std::array<sf::Vector2f, MouseButton::ButtonCount> dragStartTable;
    static std::array<sf::Vector2f, MouseButton::ButtonCount> dragLastTable;
    static std::array<sf::Vector2f, MouseButton::ButtonCount> dragDeltaTable;
    static std::array<sf::Vector2f, MouseButton::ButtonCount> dragTotalTable;

    static sf::Vector2i mousePosition = sf::Vector2i();
    static sf::Vector2f worldPosition = sf::Vector2f();
    static float mouseScrollDetla = 0.0f;
    static sf::String textEntered = "";
    static bool anyInputBool = false;
    static float dblClkDelay = 0.25f;

    static bool anyKeyDownBool = false;
    static bool anyKeyUpBool = false;
    static bool anyMouseDownBool = false;
    static bool anyMouseUpBool = false;

}

//==============================================================================
// PRIVATE STATIC FUNCTIONS
//==============================================================================

void Input::processEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::KeyPressed: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                keyDownTable[code] = true;
            anyInputBool = true;
            anyKeyDownBool = true;
            break;
        }
        case sf::Event::KeyReleased: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                keyUpTable[code]   = true;
            anyKeyUpBool = true;
            break;
        }
        case sf::Event::MouseButtonPressed: {
            mouseDownTable[event.mouseButton.button] = true;
            anyInputBool = true;
            anyMouseDownBool = true;
            // drag
            dragStartedTable[event.mouseButton.button] = true;
            draggingTable[event.mouseButton.button] = true;
            dragStartTable[event.mouseButton.button] = worldPosition;
            dragLastTable[event.mouseButton.button]  = worldPosition;
            // double click
            float now = Clock::time();
            if ((now - dblClkTimes[event.mouseButton.button]) < dblClkDelay) {
                dblClkTable[event.mouseButton.button] = true;
                dragStartedTable[event.mouseButton.button] = false;
                draggingTable[event.mouseButton.button] = false;
            }
            dblClkTimes[event.mouseButton.button] = now;
            break;
        }
        case sf::Event::MouseButtonReleased: {
            mouseUpTable[event.mouseButton.button] = true;
            anyMouseUpBool = true;
            // drag
            dragEndedTable[event.mouseButton.button] = true;
            draggingTable[event.mouseButton.button]  = false;
            break;
        }
        case sf::Event::MouseWheelScrolled: {
            mouseScrollDetla += event.mouseWheelScroll.delta;
            anyInputBool = true;
            break;
        }
        case sf::Event::TextEntered: {
            if (event.text.unicode > 31 && event.text.unicode < 128)
                textEntered += event.text.unicode;
            break;
        }
        default:
            break;
    }
}

void Input::updatePositions(const sf::Vector2i& mousePosition_, const sf::Vector2f& worldPosition_) {
    mousePosition = mousePosition_;
    worldPosition = worldPosition_;
    // update draggings
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        if (draggingTable[i]) {
            dragDeltaTable[i] = worldPosition - dragLastTable[i];
            dragTotalTable[i] = worldPosition - dragStartTable[i];
            dragLastTable[i]  = worldPosition;
        }
    }
}


void Input::clearState() {
    for (std::size_t i = 0; i < Key::KeyCount; ++i) {
        keyDownTable[i] = false;
        keyUpTable[i] = false;
    }
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        mouseDownTable[i] = false;
        mouseUpTable[i] = false;
        dblClkTable[i] = false;
        dragStartedTable[i] = false;
        dragEndedTable[i] = false;
    }
    mouseScrollDetla = 0;
    textEntered = "";
    anyInputBool = false;
    anyKeyDownBool = false;
    anyKeyUpBool = false;
    anyMouseDownBool = false;
    anyMouseUpBool = false;
}

//==============================================================================
// PUBLIC STATIC FUNCTIONS
//==============================================================================

bool Input::getKey(Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool Input::getKeyDown(Key key) {
    return keyDownTable[key];
}

bool Input::getKeyUp(Key key) {
    return keyUpTable[key];
}

bool Input::anyKeyDown() {
    return anyKeyDownBool;
}

bool Input::anyKeyUp() {
    return anyKeyUpBool;
}

bool Input::getMouse(MouseButton button) {
    return sf::Mouse::isButtonPressed(button);
}

bool Input::getMouseDown(MouseButton button) {
    return mouseDownTable[button];
}

bool Input::getMouseUp(MouseButton button) {
    return mouseUpTable[button];
}

bool Input::anyMouseDown() {
    return anyMouseDownBool;
}

bool Input::anyMouseUp() {
    return anyMouseUpBool;
}

bool Input::getDoubleClick(MouseButton button) {
    return dblClkTable[button];
}

bool Input::dragStarted(MouseButton button) {
    return dragStartedTable[button];
}

bool Input::dragging(MouseButton button) {
    return draggingTable[button];
}

bool Input::dragEnded(MouseButton button) {
    return dragEndedTable[button];
}

sf::Vector2f Input::dragDelta(MouseButton button) {
    return dragDeltaTable[button];
}

float Input::getScroll() {
    return mouseScrollDetla;
}

sf::Vector2i Input::getRawMousePosition() {
    return mousePosition;
}

sf::Vector2f Input::getMousePosition() {
    return worldPosition;
}

const sf::String& Input::getTextEntered() {
    return textEntered;
}

bool Input::getAnyInput() {
    return anyInputBool;
}

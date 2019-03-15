# include <EE/Engine/Systems/InputSystem.hpp>
# include <EE/Common/Print.hpp>
# include <EE/Engine/Engine.hpp>
#include <array>

namespace ee {

//==============================================================================
// GLOBALS
//==============================================================================

namespace {

Vector2i g_mousePosition;
Vector2f g_worldPosition;

float g_mouseScrollDetla;
float g_dblClkDelay;

bool g_anyKeyDownBool;
bool g_anyKeyUpBool;
bool g_anyMouseDownBool;
bool g_anyMouseUpBool;
bool g_anyInputBool;

sf::String g_textEntered;
std::array<bool, Key::KeyCount>                g_keyDownTable;
std::array<bool, Key::KeyCount>                g_keyUpTable;
std::array<bool, MouseButton::ButtonCount>     g_mouseDownTable;
std::array<bool, MouseButton::ButtonCount>     g_mouseUpTable;
std::array<bool, MouseButton::ButtonCount>     g_dblClkTable;
std::array<float, MouseButton::ButtonCount>    g_dblClkTimes;
std::array<bool, MouseButton::ButtonCount>     g_dragStartedTable;
std::array<bool, MouseButton::ButtonCount>     g_dragEndedTable;
std::array<bool, MouseButton::ButtonCount>     g_draggingTable;
std::array<Vector2f, MouseButton::ButtonCount> g_dragStartTable;
std::array<Vector2f, MouseButton::ButtonCount> g_dragLastTable;
std::array<Vector2i, MouseButton::ButtonCount> g_dragLastRawTable;
std::array<Vector2f, MouseButton::ButtonCount> g_dragDeltaTable;
std::array<Vector2i, MouseButton::ButtonCount> g_dragDeltaRawTable;
std::array<Vector2f, MouseButton::ButtonCount> g_dragTotalTable;

} // private namespace

//==============================================================================
// USER API
//==============================================================================

namespace Input {

bool getKey(Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool getKeyDown(Key key) {
    return g_keyDownTable[key];
}

bool getKeyUp(Key key) {
    return g_keyUpTable[key];
}

bool anyKeyDown() {
    return g_anyKeyDownBool;
}

bool anyKeyUp() {
    return g_anyKeyUpBool;
}

bool getMouse(MouseButton button) {
    return sf::Mouse::isButtonPressed(button);
}

bool getMouseDown(MouseButton button) {
    return g_mouseDownTable[button];
}

bool getMouseUp(MouseButton button) {
    return g_mouseUpTable[button];
}

bool anyMouseDown() {
    return g_anyMouseDownBool;
}

bool anyMouseUp() {
    return g_anyMouseUpBool;
}

bool getDoubleClick(MouseButton button) {
    return g_dblClkTable[button];
}

bool dragStarted(MouseButton button) {
    return g_dragStartedTable[button];
}

bool dragging(MouseButton button) {
    return g_draggingTable[button];
}

bool dragEnded(MouseButton button) {
    return g_dragEndedTable[button];
}

Vector2f dragDelta(MouseButton button) {
    return g_dragDeltaTable[button];
}

Vector2i dragDeltaRaw(MouseButton button) {
    return g_dragDeltaRawTable[button];
}

float getScroll() {
    return g_mouseScrollDetla;
}

Vector2i getRawMousePosition() {
    return g_mousePosition;
}

Vector2f getMousePosition() {
    return g_worldPosition;
}

Vector2f getMousePosition(const View& view) {
    return Engine::window->mapPixelToCoords(g_mousePosition, view);
}

const sf::String& getTextEntered() {
    return g_textEntered;
}

bool anyInput() {
    return g_anyInputBool;
}

void clearState() {
    for (std::size_t i = 0; i < Key::KeyCount; ++i) {
        g_keyDownTable[i] = false;
        g_keyUpTable[i] = false;
    }
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        g_mouseDownTable[i] = false;
        g_mouseUpTable[i] = false;
        g_dblClkTable[i] = false;
        g_dragStartedTable[i] = false;
        g_dragEndedTable[i] = false;
        g_dragDeltaTable[i] = Vector2f();
        g_dragDeltaRawTable[i] = Vector2i();
    }
    g_mouseScrollDetla = 0;
    g_textEntered = "";
    g_anyInputBool = false;
    g_anyKeyDownBool = false;
    g_anyKeyUpBool = false;
    g_anyMouseDownBool = false;
    g_anyMouseUpBool = false;
}

//==============================================================================
// DETAIL
//==============================================================================

namespace detail {

void init() {
    g_mouseScrollDetla = 0.0f;
    g_dblClkDelay = 0.30f;
    g_anyKeyDownBool = false;
    g_anyKeyUpBool = false;
    g_anyMouseDownBool = false;
    g_anyMouseUpBool = false;
    g_anyInputBool = false;
    g_textEntered = "";
    g_keyDownTable.fill(false);
    g_keyUpTable.fill(false);
    g_mouseDownTable.fill(false);
    g_mouseUpTable.fill(false);
    g_dblClkTable.fill(false);
    g_dblClkTimes.fill(0.0f);
    g_dragStartedTable.fill(false);
    g_dragEndedTable.fill(false);
    g_draggingTable.fill(false);
    g_dragStartTable.fill(Vector2f());
    g_dragLastTable.fill(Vector2f());
    g_dragLastRawTable.fill(Vector2i());
    g_dragDeltaTable.fill(Vector2f());
    g_dragDeltaRawTable.fill(Vector2i());
    g_dragTotalTable.fill(Vector2f());
}

void processEvent(const Event& event) {
    switch (event.type) {
        case Event::KeyPressed: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                g_keyDownTable[code] = true;
            g_anyInputBool = true;
            g_anyKeyDownBool = true;
            break;
        }
        case Event::KeyReleased: {
            auto code = event.key.code;
            if (code > -1 && code < Key::KeyCount)
                g_keyUpTable[code]   = true;
            g_anyKeyUpBool = true;
            break;
        }
        case Event::MouseButtonPressed: {
            g_mouseDownTable[event.mouseButton.button] = true;
            g_anyInputBool = true;
            g_anyMouseDownBool = true;
            // drag
            g_dragStartedTable[event.mouseButton.button] = true;
            g_draggingTable[event.mouseButton.button] = true;
            g_dragStartTable[event.mouseButton.button] = g_worldPosition;
            g_dragLastTable[event.mouseButton.button]  = g_worldPosition;
            g_dragLastRawTable[event.mouseButton.button] = g_mousePosition;
            // double click
            float now = Engine::time();
            if ((now - g_dblClkTimes[event.mouseButton.button]) < g_dblClkDelay) {
                g_dblClkTable[event.mouseButton.button] = true;
                g_dragStartedTable[event.mouseButton.button] = false;
                g_draggingTable[event.mouseButton.button] = false;
            }
            g_dblClkTimes[event.mouseButton.button] = now;
            break;
        }
        case Event::MouseButtonReleased: {
            g_mouseUpTable[event.mouseButton.button] = true;
            g_anyMouseUpBool = true;
            // drag
            g_dragEndedTable[event.mouseButton.button] = true;
            g_draggingTable[event.mouseButton.button]  = false;
            break;
        }
        case Event::MouseWheelScrolled: {
            g_mouseScrollDetla += event.mouseWheelScroll.delta;
            g_anyInputBool = true;
            break;
        }
        case Event::TextEntered: {
            if (event.text.unicode > 31 && event.text.unicode < 128)
                g_textEntered += event.text.unicode;
            break;
        }
        default:
            break;
    }
}

void update() {
    clearState();
    g_mousePosition = sf::Mouse::getPosition(*Engine::window);
    g_worldPosition = Engine::window->mapPixelToCoords(g_mousePosition, Engine::getView(0));
    // update draggings
    for (std::size_t i = 0; i < MouseButton::ButtonCount; ++i) {
        if (g_draggingTable[i]) {
            g_dragDeltaTable[i]    = g_worldPosition - g_dragLastTable[i];
            g_dragDeltaRawTable[i] = g_mousePosition - g_dragLastRawTable[i];
            g_dragTotalTable[i]    = g_worldPosition - g_dragStartTable[i];
            g_dragLastTable[i]     = g_worldPosition;
            g_dragLastRawTable[i]  = g_mousePosition;
        }
    }
}

} // namespace detail
} // namespace Input
} // namespace ee

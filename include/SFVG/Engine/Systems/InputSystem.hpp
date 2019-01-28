#pragma once

#include <SFVG/Common/Imports.hpp>
#include <SFVG/Engine/System.hpp>
#include <array>

namespace sfvg {

class InputSystem : private System {
public:

    InputSystem(Engine& engine, const Name& name);

    bool getKey(Key key);
    bool getKeyDown(Key key);
    bool getKeyUp(Key key);
    bool anyKeyDown();
    bool anyKeyUp();

    bool getMouse(MouseButton button);
    bool getMouseDown(MouseButton button);
    bool getMouseUp(MouseButton button);
    bool anyMouseDown();
    bool anyMouseUp();

    bool getDoubleClick(MouseButton button);
    float getScroll();

    bool dragStarted(MouseButton button);
    bool dragging(MouseButton button);
    bool dragEnded(MouseButton button);
    Vector2f dragDelta(MouseButton button);

    Vector2i getRawMousePosition();
    Vector2f getMousePosition();
    Vector2f getMousePosition(const View& view);

    const sf::String& getTextEntered();

    bool anyInput();
    void clearState();

private:

    friend class Engine;

    void update() override;
    void processEvent(const Event& event) override;

private:

    Vector2i m_mousePosition;
    Vector2f m_worldPosition;

    float m_mouseScrollDetla;
    float m_dblClkDelay;

    bool m_anyKeyDownBool;
    bool m_anyKeyUpBool;
    bool m_anyMouseDownBool;
    bool m_anyMouseUpBool;
    bool m_anyInputBool;

    sf::String m_textEntered;

    std::array<bool, Key::KeyCount>                    m_keyDownTable;
    std::array<bool, Key::KeyCount>                    m_keyUpTable;

    std::array<bool, MouseButton::ButtonCount>         m_mouseDownTable;
    std::array<bool, MouseButton::ButtonCount>         m_mouseUpTable;
    std::array<bool, MouseButton::ButtonCount>         m_dblClkTable;

    std::array<float, MouseButton::ButtonCount>        m_dblClkTimes;

    std::array<bool, MouseButton::ButtonCount>         m_dragStartedTable;
    std::array<bool, MouseButton::ButtonCount>         m_dragEndedTable;
    std::array<bool, MouseButton::ButtonCount>         m_draggingTable;

    std::array<Vector2f, MouseButton::ButtonCount>    m_dragStartTable;
    std::array<Vector2f, MouseButton::ButtonCount>     m_dragLastTable;
    std::array<Vector2f, MouseButton::ButtonCount>     m_dragDeltaTable;
    std::array<Vector2f, MouseButton::ButtonCount>     m_dragTotalTable;
};

} // namespace sfvg

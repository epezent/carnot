#pragma once

#include <SFVG/Imports.hpp>

namespace sfvg {

class Engine;

class Input {
public:
    static bool getKey(Key key);
    static bool getKeyDown(Key key);
    static bool getKeyUp(Key key);

    static bool anyKeyDown();
    static bool anyKeyUp();

    static bool getMouse(MouseButton button);
    static bool getMouseDown(MouseButton button);
    static bool getMouseUp(MouseButton button);

    static bool anyMouseDown();
    static bool anyMouseUp();

    static bool getDoubleClick(MouseButton button);
    static float getScroll();

    static bool dragStarted(MouseButton button);
    static bool dragging(MouseButton button);
    static bool dragEnded(MouseButton button);
    static Vector2f dragDelta(MouseButton button);

    static Vector2i getRawMousePosition();
    static Vector2f getMousePosition();

    static const sf::String& getTextEntered();

    static bool getAnyInput();

private:
    friend class Engine;

    static void processEvent(const Event& event);
    static void updatePositions(const Vector2i& mousePosition, const Vector2f& worldPosition);
    static void clearState();
};

} // namespace sfvg

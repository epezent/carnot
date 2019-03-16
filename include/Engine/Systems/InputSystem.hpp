#pragma once

#include <Common/Imports.hpp>

namespace carnot {
namespace Input {

/// Returns true if the Key is currently pressed    
bool getKey(Key key);
/// Returns true if the Key was pressed this frame
bool getKeyDown(Key key);
/// Returns true if the Key was released this frame
bool getKeyUp(Key key);
/// Returns true if any Key was pressed this frame
bool anyKeyDown();
///  Returns true if any Key was release this frame
bool anyKeyUp();

/// Returns true if the MouseButton is currently pressed
bool getMouse(MouseButton button);
/// Returns true if the MouseButton was pressed this frame
bool getMouseDown(MouseButton button);
/// Returns true if the MouseButton was released this frame
bool getMouseUp(MouseButton button);
/// Returns true if any MouseButton was pressed this frame
bool anyMouseDown();
/// Returns true if any MouseButton was release this frame
bool anyMouseUp();

/// Returns true if the MouseButton was double-clicked this frame
bool getDoubleClick(MouseButton button);
/// Returns the mmouse scroll amount for this frame
float getScroll();

/// Returns true if a mouse drag was started this frame
bool dragStarted(MouseButton button);
/// Returns true if a mouse drag is currently occuring
bool dragging(MouseButton button);
/// Returns true if a mouse drag ended this frame
bool dragEnded(MouseButton button);
/// Returns the amount of delta drag for this frame in world units
Vector2f dragDelta(MouseButton button);
/// Returns the amount of delta drag for this frame in pixel units
Vector2i dragDeltaRaw(MouseButton button);

/// Returns the mouse position in world units relative to the default view
Vector2f getMousePosition();
/// Returns the mouse position in world units relative to a specific view
Vector2f getMousePosition(const View& view);
/// Returns the mouse position in pixel units releative to the window
Vector2i getRawMousePosition();

/// Returns user text input for this frame
const sf::String& getTextEntered();

/// Returns true if any user input was registered this frame
bool anyInput();
/// Allows the user to manually clear the Input state within a frame
void clearState();

// Implementation details [internal use only]
namespace detail {
void init();
void update();
void processEvent(const Event& event);
} // namespace detail
} // namespace Input
} // namespace carnot

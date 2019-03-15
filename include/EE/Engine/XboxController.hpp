#pragma once

#include <memory>

namespace ee {

/// Encapsulates an Xbox 360 or Xbox One Controller
class XboxController {
public:
    /// Represents an Xbox Controller Button
    enum Button {
        A,      ///< A button
        B,      ///< B button
        X,      ///< X button
        Y,      ///< Y button
        LB,     ///< Left bumper
        RB,     ///< Right bumper
        LS,     ///< Left stick
        RS,     ///< Right stick
        Up,     ///< D-pad up
        Down,   ///< D-pad down
        Left,   ///< D-pad left
        Right,  ///< D-pad right
        Back,   ///< Back button
        Start   ///< Start button
    };

    /// Represents an Xbox Controller Axis
    enum Axis {
        LX = 0,  ///< Left stick x-axis
        LY = 1,  ///< Left stick y-axis
        LT = 2,  ///< Left trigger
        RX = 3,  ///< Right stick x-axis
        RY = 4,  ///< Right stick y-axis
        RT = 5   ///< Righter trigger
    };

public:
    /// Constructor
    XboxController(int instance = 0);

    /// Destructor
    ~XboxController();

    /// Returns true if the controller is connected
    bool isConnected();

    /// Returns true if the is held
    bool getButton(Button button);

    /// Returns true if the button was pressed (updateEvents() must be called)
    bool getButtonDown(Button button);

    /// Returns true if the button was released (updateEvents() must be called)
    bool getButtonUp(Button button);

    /// Returns current state of axsis in range -1.0 (or 0.0) to 1.0
    float getAxis(Axis axis);

    /// Sets the deadzone of an axis in range 0.0 to 1.0
    void setDeadzone(Axis axis, float deadzone = 0.0);

    /// Vibriates the controller in range 0.0 to 1.0
    void vibrate(float left_motor = 0.0, float right_motor = 0.0);

    /// Updates button up/down events (call once per frame)
    void updateEvents();

private:
    class Impl;                   ///< Pimpl idiom
    std::unique_ptr<Impl> impl_;  ///< OS-specific implementation
};

}  // namespace ee

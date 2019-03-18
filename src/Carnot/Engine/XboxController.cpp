#include <Windows.h>
#include <Xinput.h>
#include <Engine/XboxController.hpp>
#include <Common/Math.hpp>
#include <array>

namespace carnot {

//=============================================================================
// PIMPL IDIOM
//=============================================================================

class XboxController::Impl {
public:
    Impl(int controller_number);

    ~Impl();

    void updateState();

    bool getButton(Button button);

    float getAxis(Axis axis);

    void setDeadzone(Axis axis, float deadzone);

    bool isConnected();

    void vibrate(float left_motor, float right_motor);

    int controller_number_;

    std::array<float, 6> deadzones_;

    XINPUT_STATE state_;
};

XboxController::Impl::Impl(int controller_number)
    : controller_number_(controller_number), deadzones_({0, 0, 0, 0, 0, 0}) {}

XboxController::Impl::~Impl() {}

void XboxController::Impl::updateState() {
    ZeroMemory(&state_, sizeof(XINPUT_STATE));
    XInputGetState(controller_number_, &state_);
}

bool XboxController::Impl::getButton(Button button) {
    switch (button) {
        case Button::A:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_A);
        case Button::B:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_B);
        case Button::X:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_X);
        case Button::Y:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
        case Button::LB:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        case Button::RB:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        case Button::LS:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        case Button::RS:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        case Button::Up:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        case Button::Down:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        case Button::Left:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        case Button::Right:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        case Button::Back:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
        case Button::Start:
            return (state_.Gamepad.wButtons & XINPUT_GAMEPAD_START);
    }
    return false;
}

float XboxController::Impl::getAxis(Axis axis) {
    float norm, value;
    switch (axis) {
        case Axis::LX:
            norm = std::max(-1.0, (float)state_.Gamepad.sThumbLX / 32767.0);
            value =
                (std::abs(norm) < deadzones_[LX]
                     ? 0
                     : (std::abs(norm) - deadzones_[LX]) * (norm / abs(norm)));
            if (deadzones_[LX] > 0)
                value /= 1 - deadzones_[LX];
            return value;
        case Axis::LY:
            norm = std::max(-1.0, (float)state_.Gamepad.sThumbLY / 32767.0);
            value =
                (std::abs(norm) < deadzones_[LY]
                     ? 0
                     : (std::abs(norm) - deadzones_[LY]) * (norm / abs(norm)));
            if (deadzones_[LY] > 0)
                value /= 1 - deadzones_[LY];
            return value;
        case Axis::LT:
            return (float)state_.Gamepad.bLeftTrigger / 255.0;
        case Axis::RX:
            norm = std::max(-1.0, (float)state_.Gamepad.sThumbRX / 32767.0);
            value =
                (std::abs(norm) < deadzones_[RX]
                     ? 0
                     : (std::abs(norm) - deadzones_[RX]) * (norm / abs(norm)));
            if (deadzones_[RX] > 0)
                value /= 1 - deadzones_[RX];
            return value;
        case Axis::RY:
            norm = std::max(-1.0, (float)state_.Gamepad.sThumbRY / 32767.0);
            value =
                (std::abs(norm) < deadzones_[RY]
                     ? 0
                     : (std::abs(norm) - deadzones_[RY]) * (norm / abs(norm)));
            if (deadzones_[RY] > 0)
                value /= 1 - deadzones_[RY];
            return value;
        case Axis::RT:
            return (float)state_.Gamepad.bRightTrigger / 255.0;
    }
    return 0.0;
}

void XboxController::Impl::setDeadzone(Axis axis, float deadzone) {
    deadzones_[axis] = deadzone;
}

bool XboxController::Impl::isConnected() {
    ZeroMemory(&state_, sizeof(XINPUT_STATE));
    DWORD Result = XInputGetState(controller_number_, &state_);
    if (Result == ERROR_SUCCESS)
        return true;
    else
        return false;
}

void XboxController::Impl::vibrate(float left_motor, float right_motor) {
    left_motor = Math::clamp01(left_motor);
    right_motor = Math::clamp01(right_motor);
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed  = static_cast<WORD>(65535 * left_motor);
    vibration.wRightMotorSpeed = static_cast<WORD>(65535 * right_motor);
    XInputSetState(controller_number_, &vibration);
}

//=============================================================================
// CLASS DEFINITIONS
//=============================================================================

XboxController::XboxController(int instance)
    : impl_(new XboxController::Impl(instance)) {}

XboxController::~XboxController() {}

bool XboxController::getButton(Button button) {
    impl_->updateState();
    return impl_->getButton(button);
}

float XboxController::getAxis(Axis axis) {
    impl_->updateState();
    return impl_->getAxis(axis);
}

void XboxController::setDeadzone(Axis axis, float deadzone) {
    impl_->setDeadzone(axis, deadzone);
}

bool XboxController::isConnected() {
    return impl_->isConnected();
}

void XboxController::vibrate(float left_motor, float right_motor) {
    return impl_->vibrate(left_motor, right_motor);
}

void XboxController::updateEvents() {

}

}  // namespace mel

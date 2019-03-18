#pragma once

#include <Graphics/Effect.hpp>
#include <Graphics/Color.hpp>

namespace carnot {

class Checkerboard : public Effect {
public:
    Checkerboard(const Color &color1 = Color::White,
                 const Color &color2 = Color::Black,
                 int checks = 10,
                 float ratio   = 1.0f);

public:

    Color color1;
    Color color2;
    int checks;
    float ratio;

private:
    Shader *shader() const override;

private:
    Shader *m_shader;
};

} // namespace carnot

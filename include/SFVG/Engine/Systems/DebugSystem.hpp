#pragma once

#include <SFVG/Engine/System.hpp>
#include <SFVG/Imports.hpp>

namespace sfvg {

class DebugSystem : public System {
public:

    void showInfo(bool show);

private:

    bool m_showInfo;

    Text m_infoText;
    Text m_pauseText;

};

} // namespace sfvg

#pragma once

#include <Engine/Component.hpp>

namespace carnot {

class ParticleSystem : public Component {

    /// Constructor
    ParticleSystem(GameObject& gameObject);
    /// Destructor
    ~ParticleSystem();

};


} // namespace carnot
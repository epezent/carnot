#pragma once

#include <Engine/Component.hpp>

class b2ParticleSystem;

namespace carnot {

class ParticleSystem : public Component {
public:
    /// Constructor
    ParticleSystem(GameObject& gameObject);
    /// Destructor
    ~ParticleSystem();

public:

    static std::size_t getParticleCount();

private:
    b2ParticleSystem* m_system;
};


} // namespace carnot
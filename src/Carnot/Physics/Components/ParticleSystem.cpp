#include <Physics/PhysicsSystem.hpp>
#include <Physics/Components/ParticleSystem.hpp>
#include <Engine/Engine.hpp>
#include <Carnot/Glue/Box2D.inl>

namespace carnot {

ParticleSystem::ParticleSystem(GameObject& _gameObject) :
    Component(_gameObject)
{ 
    b2ParticleSystemDef def;
    def.radius =  Physics::detail::scale() * 5;
    def.pressureStrength = 1.0f;
    def.springStrength = 1;
    def.elasticStrength = 1;
    
    m_system = Physics::detail::world()->CreateParticleSystem(&def);
    b2ParticleDef pdef;
    for (int i = -2500; i < 2500; ++i) {
        pdef.position.x = i * Physics::detail::scale() * 0.25;
        m_system->CreateParticle(pdef);
    }
}

ParticleSystem::~ParticleSystem() {

}

std::size_t ParticleSystem::getParticleCount() {
    std::size_t count = 0;
    for (auto sys = Physics::detail::world()->GetParticleSystemList();sys;sys = sys->GetNext())
        count += sys->GetParticleCount();
    return count;
}

} // namespace carnot
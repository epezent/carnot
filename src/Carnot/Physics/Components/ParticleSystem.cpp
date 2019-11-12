#include <Physics/PhysicsSystem.hpp>
#include <Physics/Components/ParticleSystem.hpp>
#include <Engine/Engine.hpp>
#include <Carnot/Glue/Box2D.inl>

namespace carnot {

ParticleSystem::ParticleSystem(GameObject& _gameObject) :
    Component(_gameObject)
{ 
    b2ParticleSystemDef def;
    Physics::detail::world()->CreateParticleSystem(&def);
}

ParticleSystem::~ParticleSystem() {

}

} // namespace carnot
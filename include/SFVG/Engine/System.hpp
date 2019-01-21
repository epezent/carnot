#pragma once

#include <SFVG/Engine/Object.hpp>

namespace sfvg {

class Engine;

class System : public Object {
public:

    /// Constructor
    System(Engine& engine, const Name& name);

protected:

    friend class Engine;

    virtual void processEvent(const Event& event);

};

} // namespace sfvg

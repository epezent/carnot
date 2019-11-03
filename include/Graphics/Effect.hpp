#pragma once
#include <Utility/Types.hpp>

namespace carnot
{

/// Provides a convientient wrapper around a shader effect
class Effect {
public:

    /// Virtual destructor
    ~Effect();

    /// Sets the state of the underlying shader and returns it
    virtual Shader* shader() const = 0;

};
    
} // namespace carnot

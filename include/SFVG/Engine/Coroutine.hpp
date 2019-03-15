#pragma once

#include <experimental/coroutine>
#include <experimental/generator>
#include <SFVG/Common/Imports.hpp>
#include <SFVG/Engine/Handle.hpp>

namespace sfvg {

//==============================================================================
// Forward Declarations / Typedefs
//==============================================================================

class Enumerator;
class Object;
using SuspendAlawys = std::experimental::suspend_always;
using SuspendNever  = std::experimental::suspend_never;

//==============================================================================
// YieldInsruction
//==============================================================================

/// Coroutine yield instruction base class
struct YieldInstruction {
    YieldInstruction();
    virtual ~YieldInstruction();
    virtual bool isOver();
};

/// Yield instruction which waits a certain duration in seconds
struct WaitForSeconds : public YieldInstruction {
    WaitForSeconds(float duration);
    bool isOver() override;
private:
    float m_duration;
    float m_elapsedTime;
};

//==============================================================================
// PromiseType
//==============================================================================

struct PromiseType {
    PromiseType();
    ~PromiseType();
    SuspendAlawys initial_suspend();
    SuspendAlawys final_suspend();
    Enumerator get_return_object();
    void unhandled_exception();
    SuspendNever  return_void();
    SuspendAlawys yield_value(YieldInstruction* value);
    SuspendAlawys yield_value(Ptr<YieldInstruction> value);
    SuspendAlawys yield_value(Handle<YieldInstruction> value);
    Ptr<YieldInstruction> m_instruction;
};

//==============================================================================
// Coroutine
//==============================================================================

struct Coroutine : public YieldInstruction, private NonCopyable
{

    /// Destructor
    ~Coroutine();
    /// Stops the Coroutine
    void stop();
    /// Returns true if the Coroutine is over
    bool isOver() override;
    /// Move semantics
    Coroutine(Coroutine &&other);

private:

    friend class Object;
    friend class Enumerator;
    friend struct PromiseType;

    /// Constructor
    Coroutine(std::experimental::coroutine_handle<PromiseType> coroutine);

private:
    std::experimental::coroutine_handle<PromiseType> m_coroutine; ///< underlying handle
    bool m_stop;
};

//==============================================================================
// Enumerator
//==============================================================================

class Enumerator : private NonCopyable {
public:

    using promise_type = PromiseType;

    /// Destructor
    ~Enumerator();
    /// Move semantics
    Enumerator(Enumerator &&e);
    /// Advances Enumerator and returns true until completion
    bool moveNext();

private:

    friend class Object;
    friend class Component;
    friend struct PromiseType;

    /// Constructor
    Enumerator(Ptr<Coroutine> h);
    /// Gets the Coroutine
    Handle<Coroutine> getCoroutine();

private:
    Ptr<Coroutine> m_ptr;
};

} // namespace sfvg

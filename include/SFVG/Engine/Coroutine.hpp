#pragma once

#include <experimental/coroutine>
#include <experimental/generator>
#include <SFVG/Print.hpp>
#include <SFVG/Imports.hpp>
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
    Time m_duration;
    Clock m_clock;
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
    Ptr<YieldInstruction> m_currentValue;
};

//==============================================================================
// Coroutine
//==============================================================================

using CoroutineHandle = std::experimental::coroutine_handle<PromiseType>;

struct Coroutine : public YieldInstruction
{
    bool isOver() override;

private:

    friend class Enumerator;
    Coroutine(CoroutineHandle coroutine);

private:
    CoroutineHandle m_coroutine;
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

private:

    friend class Object;
    friend struct PromiseType;

    /// Constructor
    Enumerator(CoroutineHandle h);
    /// Advance Enumerator, returns true if coroutine not done
    bool next();
    /// Gets the current YieldInstruction
    Handle<YieldInstruction> currentValue();

    Handle<Coroutine> getCoroutine();

private:
    CoroutineHandle m_coroutine;  ///< coroutine handle
    Ptr<Coroutine> m_ptr;
};

} // namespace sfvg

#include <SFVG/Engine/Coroutine.hpp>

namespace sfvg {

//==============================================================================
// YieldInstruction
//==============================================================================

YieldInstruction::YieldInstruction() {
    print("YieldInstruction Created");
}

YieldInstruction::~YieldInstruction() {
    print("YieldInstruction Destroyed");
}

bool YieldInstruction::isOver() {
   return true;
}

WaitForSeconds::WaitForSeconds(float duration) :
   YieldInstruction(),
   m_duration(seconds(duration))
{
}

bool WaitForSeconds::isOver() {
   return m_clock.getElapsedTime() >= m_duration;
}

//==============================================================================
// PromiseType
//==============================================================================

PromiseType::PromiseType() :
   m_currentValue(nullptr)
{
   print("PROMISE CONSTRUCTED");
}

PromiseType::~PromiseType() {
   print("PROMISE DESTRUCTED");
}

SuspendAlawys PromiseType::initial_suspend() {
   print("CORO STARTED");
   return SuspendAlawys{}; // changing this will make coroutine start immmediately or after update
}

SuspendAlawys PromiseType::final_suspend() {
   print("CORO FINISHED");
   return SuspendAlawys{};
}

Enumerator PromiseType::get_return_object() {
   return Enumerator{ CoroutineHandle::from_promise(*this) };
}

void PromiseType::unhandled_exception() {
   std::exit(1);
}

SuspendNever PromiseType::return_void() {
   return SuspendNever{};
}

SuspendAlawys PromiseType::yield_value(YieldInstruction* value) {
   m_currentValue = std::shared_ptr<YieldInstruction>(value);
   return SuspendAlawys{};
}

SuspendAlawys PromiseType::yield_value(Ptr<YieldInstruction> value) {
    m_currentValue = value;
    return SuspendAlawys{};
}

SuspendAlawys PromiseType::yield_value(Handle<YieldInstruction> value) {
    m_currentValue = value.lock();
    return SuspendAlawys{};
}

//==============================================================================
// Coroutine
//==============================================================================

Coroutine::Coroutine(CoroutineHandle coroutine) :
    YieldInstruction(),
    m_coroutine(coroutine)
{
    
}

bool Coroutine::isOver() {
    return m_coroutine.done();
}

//==============================================================================
// Enumerator
//==============================================================================

bool Enumerator::next() {
   m_coroutine.resume();
   return !m_coroutine.done();
}

Handle<YieldInstruction> Enumerator::currentValue() {
    return m_coroutine.promise().m_currentValue;
}

Enumerator::Enumerator(CoroutineHandle h)
    : m_coroutine(h),
    m_ptr(new Coroutine(h))
{
}

Enumerator::~Enumerator() {
    if (m_coroutine) {
        m_coroutine.destroy();
        print("Coroutine Handle Destroyed");
    }
}

Enumerator::Enumerator(Enumerator &&e) :
    m_coroutine(e.m_coroutine),
    m_ptr(e.m_ptr)
{
    e.m_coroutine = nullptr;
    e.m_ptr = nullptr;
};

Handle<Coroutine> Enumerator::getCoroutine() {
    return m_ptr;
}

} // namespac sfvg

//==============================================================================
// Enumerator (Old)
//==============================================================================

/*
class Enumerator {
public:

    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;
    struct promise_type {

        promise_type() :
            m_currentValue(nullptr)
        {
        }

        ~promise_type() {
            if (m_currentValue)
                delete m_currentValue;
        }

        auto initial_suspend() {
            return std::experimental::suspend_always{}; // changing this will make coroutine start immmediately or after update
        }

        auto final_suspend() {
            return std::experimental::suspend_always{};
        }

        auto get_return_object() {
            return Enumerator{ coroutine_handle::from_promise(*this) };
        }

        void unhandled_exception() {
            std::exit(1);
        }

        auto return_void() {
            return std::experimental::suspend_never{};
        }

        auto yield_value(YieldInstruction* value) {
            if (m_currentValue) {
                delete m_currentValue;
            }
            m_currentValue = value;
            return std::experimental::suspend_always{};
        }
        YieldInstruction* m_currentValue;
    };


    bool next() {
        coro.resume();
        return !coro.done();
    }

    YieldInstruction* currentValue() {
        return coro.promise().m_currentValue;
    }

    Enumerator(coroutine_handle h)
        : coro(h)
    {
    }

    ~Enumerator() {
        if (coro)
            coro.destroy();
    }

    Enumerator(const Enumerator &) = delete;
    Enumerator(Enumerator &&g)
        : coro(g.coro) {
        g.coro = nullptr;
    };

    coroutine_handle coro;
};
*/

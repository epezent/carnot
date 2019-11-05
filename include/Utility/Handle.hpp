#pragma once

#include <memory>
#include <cassert>

namespace carnot {

/// Alias std::shared_ptr for convenience
template <typename T>
using Ptr = std::shared_ptr<T>;

/// Alis std::weak_ptr for convenience
template <typename T>
using WkPtr = std::weak_ptr<T>;

/// Shorthand for std::make_shared<T>(...)
template <typename T, typename ...Args>
inline Ptr<T> make(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

//==============================================================================
// HANDLE
//==============================================================================

/// Handle class
template <typename T>
class Handle {
public:


    /// Constructs an invalid Handle
    Handle();

    /// Constructs a valid Handle
    Handle(WkPtr<T> wkPtr);

    /// Constructs a valid Handle from another Handle
    template <typename U> Handle(Handle<U> handle);

    /// Access Object held by Handle
    T* operator->() const;

    /// Returns true if the Object pointed to is still alive
    bool isValid() const;

    /// Returns true if the Object pointed to is still alive
    operator bool() const;

    /// Returns this Handle cast as another Handle type
    template <typename U> Handle<U> as();

    /// Returns a shared pointer from Handle
    Ptr<T> lock() const;

    /// Get underlying raw pointer (use sparingly!)
    T* get() const;

private:

    WkPtr<T> m_wkPtr;  ///< weak pointer to Object

};

/// Determine if two Handles of the same type point to the same T*
template <typename T>
bool operator ==(const Handle<T>& lhs, const Handle<T>& rhs);

/// Determine if two Handles of the same type do not point to the same T*
template <typename T>
bool operator !=(const Handle<T>& lhs, const Handle<T>& rhs);

/// Determine if two Handles of the different type point to the same T*
template <typename T, typename U>
bool operator ==(const Handle<T>& lhs, const Handle<U>& rhs);

/// Determine if two Handles of the different type do not point to the same T*
template <typename T, typename U>
bool operator !=(const Handle<T>& lhs, const Handle<U>& rhs);

//==============================================================================
// TEMPLATE FUNCTION IMPLEMENTATIONS
//==============================================================================

template <typename T>
Handle<T>::Handle() :
    m_wkPtr()
{

}

template <typename T>
Handle<T>::Handle(WkPtr<T> wkPtr) :
    m_wkPtr(wkPtr)
{

}

template <typename T>
template <typename U>
Handle<T>::Handle(Handle<U> handle) {
    *this = handle.as<T>();
}

template <typename T>
template <typename U>
Handle<U> Handle<T>::as() {
    Ptr<U> ptr = std::dynamic_pointer_cast<U>(m_wkPtr.lock());
    return Handle<U>(ptr);
}

template <typename T>
T* Handle<T>::operator->() const {
    assert(isValid());
    return m_wkPtr.lock().get();
}

template <typename T>
Handle<T>::operator bool() const {
    return isValid();
}

template <typename T>
bool Handle<T>::isValid() const {
    return !m_wkPtr.expired() && get() != nullptr;
}

template <typename T>
T* Handle<T>::get() const {
    return m_wkPtr.lock().get();
}

template <typename T>
Ptr<T> Handle<T>::lock() const {
    return m_wkPtr.lock();
}

template <typename T>
bool operator ==(const Handle<T>& lhs, const Handle<T>& rhs) {
    return lhs.get() == rhs.get();
}

template <typename T>
bool operator !=(const Handle<T>& lhs, const Handle<T>& rhs) {
    return lhs.get() != rhs.get();
}

template <typename T, typename U>
bool operator ==(const Handle<T>& lhs, const Handle<U>& rhs) {
    return static_cast<void*>(lhs.get()) == static_cast<void*>(rhs.get());
}

template <typename T, typename U>
bool operator !=(const Handle<T>& lhs, const Handle<U>& rhs) {
    return static_cast<void*>(lhs.get()) != static_cast<void*>(rhs.get());
}

} // namespace carnot

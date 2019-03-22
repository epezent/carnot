#pragma once
#include <cstddef>

namespace carnot {

/// Base type for objects which benifit from cached states
class Cacheable {
public:

    /// Default constructor
    Cacheable(bool stale = true);

    /// Returns the age of the cache
    std::size_t cacheAge() const;

    /// Checks if an external age is current with the cache's age
    bool cacheCurrent(std::size_t& age) const;

protected:

    /// Instructs the object to update its cached state if stale
    void updateCacheIfStale() const;

    /// User define cahce update implementation
    virtual void onCacheUpdate() const = 0;

    /// Flags the cached state as stale
    void makeCacheStale() const;

private:
    mutable std::size_t m_age; ///< age of cache, increments with each update
    mutable bool m_stale;      ///< true if the cache has become invalidated
};
    
} // carnot

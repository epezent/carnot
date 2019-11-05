#include <Utility/Cacheable.hpp>
#include <Utility/Print.hpp>
#include <cassert>

namespace carnot
{
    
Cacheable::Cacheable(bool stale) :
    m_age(0),
    m_stale(stale)
{

}

std::size_t Cacheable::cacheAge() const {
    return m_age;
}

bool Cacheable::cacheCurrent(std::size_t& age) const {
    updateCacheIfStale();
    if (age == m_age)
        return true;
    else {
        age = m_age;
        return false;
    }
}

void Cacheable::updateCacheIfStale() const {
    if (m_stale) {
        onCacheUpdate();
        m_age++;
        m_stale = false;
    }    
}

void Cacheable::onCacheUpdate() const {
    // do nothing by default
}

void Cacheable::makeCacheStale() const {
    m_stale = true;
}

} // carnot

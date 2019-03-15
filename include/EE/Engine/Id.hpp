#pragma once

#include <string>

namespace ee {

/// Unique identifier for Engine types
typedef std::size_t Id;
typedef std::string Name;

namespace ID {

/// Makes a new ID for a string Name
extern Id makeId(const Name& name);

/// Frees an ID for future use if it exists
extern void freeId(Id id);

/// Returns an existing unique Id for a string Name
extern Id getId(const Name& name);

/// Gets the string name associated with an Id if it exists
extern const Name& getName(Id id);

} // namespace ID

} // namespace ee

#pragma warning (disable : 4100)

#include <SFVG/Engine/System.hpp>
#include <SFVG/Engine/Engine.hpp>

namespace sfvg {

System::System(Engine& engine, const Name& name) :
    Object(engine, name)
{

}

void System::processEvent(const Event& event) {
    // do nothing by default
}


} // namespace sfvg

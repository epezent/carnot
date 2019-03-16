#include <carnot>

using namespace carnot;

int main(int argc, char const *argv[])
{
    Engine::init(500,500);
    Engine::makeRoot<GameObject>();
    Engine::run();
    return 0;
}

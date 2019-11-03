#include <carnot>
using namespace carnot;

int main(int argc, char const *argv[])
{
    Engine::init(250,250,"Hello, World");
    Engine::makeRoot<GameObject>();
    Engine::run();
    return 0;
}

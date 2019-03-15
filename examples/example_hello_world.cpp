# include <EE/EE.hpp>

using namespace ee;

int main(int argc, char const *argv[])
{
    Engine engine(500,500);
    engine.makeRoot<GameObject>();
    engine.run();
    return 0;
}

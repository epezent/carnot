#include <carnot>

using namespace carnot;

static void method0(float t) {
    print("static method0:", t);
}

class SignalsDemo : public GameObject {
public:

    Signal<void(float)> onSpace;

    void start() {
        onSpace.connect(method0);
        onSpace.connect(method1);
        onSpace.connect(this, &SignalsDemo::method2);
    }

    void update() {
        if (Input::getKeyDown(Key::Space))
            onSpace.emit(Engine::time());
    }

    static void method1(float t) {
        print("static method1:", t);
    }

    void method2(float t) {
        print(getName(), "method2:", t);
    }

};

int main(int argc, char const *argv[])
{
    Engine::init(250,250,"Signals");
    Engine::makeRoot<SignalsDemo>();
    Engine::run();
    
    return 0;
}
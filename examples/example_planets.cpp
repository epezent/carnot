#include <carnot>

using namespace carnot;

class Body : public GameObject {
public:
    Body(const std::string& name, Color color, float bodyRadius, float orbitRadius, float orbitSpeed) :
        orbitSpeed(orbitSpeed)
    {
        transform.setLocalPosition(0,0);
        body = makeChild<GameObject>(name);
        body->transform.setLocalPosition(orbitRadius, 0);
        body->addComponent<ShapeRenderer>(make<CircleShape>(bodyRadius))->setColor(color);
        showGizmos = false;
    } 
    
    void update() {
        transform.rotate(orbitSpeed * Engine::deltaTime());
    }

    Handle<GameObject> body; ///< dummy object that holds graphics
    float orbitSpeed;        ///< orbit speed
};

int main(int argc, char const *argv[])
{
    Engine::init();
    Engine::getView(0).setCenter(0,0);
    auto sun     =       Engine::makeRoot<Body>("sun",     Yellows::Gold,    50.0f, 0.0f,   0.0f);
    auto mercury =   sun->body->makeChild<Body>("mercury", Purples::Lavender, 8.0f, 75.0f, 30.0f);
    auto venus   =   sun->body->makeChild<Body>("venus",   Yellows::Khaki, 14.0f, 125.0f, 12.0f);
    auto earth   =   sun->body->makeChild<Body>("earth",   Cyans::LightSeaGreen, 15.0f, 200.0f, 10.0f);
    auto moon    = earth->body->makeChild<Body>("moon",    Grays::Gray50,   3.0f,  20.0f,  100.0f);
    auto mars    =   sun->body->makeChild<Body>("mars",    Reds::IndianRed, 10.0f, 300.0f, 5.0f);
    auto phobos  =  mars->body->makeChild<Body>("phobos",  Grays::Gray50,   3.0f,  15.0f,  100.0f);
    auto deimos  =  mars->body->makeChild<Body>("deimos",  Grays::Gray50,   2.0f,  20.0f,  50.0f);

    Engine::run();
    return 0;
}


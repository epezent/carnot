#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Fonts/RobotoBold.hpp>

template <typename P>
class PrimitiveObject : public Object {
public:

    PrimitiveObject() {}

    void update() override {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(primitive, states);
    }

    P primitive;
};

class ShapeObject : public Object {
public:
    ShapeObject() { }
    virtual ~ShapeObject() { }
    virtual void setColor(const Color& color) = 0;
};

class CrossObject : public ShapeObject {
public:

    CrossObject() {
        m_cross.setWidth(50);
        m_cross.setHeight(50);
        m_cross.setThickness(10);
        m_cross.setColor(sfvg::Grays::Black);
    }

    void setColor(const Color& color) override {
        m_cross.setColor(color);
    }

private:

    void update() override {
        setGlobalRotation(0.0f);
        setGlobalScale(1.0f, 1.0f);

        if (Input::getKeyDown(Key::R)) {
            print(getRotation());
            print(getGlobalRotation());
        }

        if (Input::getKeyDown(Key::S))
            print(getGlobalScale());

        if (Input::getKeyDown(Key::P))
            getParent().as<ShapeObject>()->setColor(sfvg::Reds::Crimson);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_cross, states);
    }

    sfvg::CrossShape m_cross;
};



class SquareObject : public ShapeObject {
public:

    SquareObject() {
        m_square.setSideLength(50);
        m_square.setColor(sfvg::Grays::Black);
    }

    void setColor(const sf::Color& color) override {
        m_square.setColor(color);
    }

private:

    void update() override {

        if (Input::getKey(Key::Right))
            rotate(90.0f * Clock::deltaTime());
        if (Input::getKey(Key::Left))
            rotate(-90.0f * Clock::deltaTime());
        if (Input::getKey(Key::Up))
            setScale(getScale() + Vector2f(1.f, 1.f) * Clock::deltaTime());
        if (Input::getKey(Key::Down))
            setScale(getScale() - Vector2f(1.f, 1.f) * Clock::deltaTime());

        if (Input::getKeyDown(Key::C)) {
            auto child = makeChild<CrossObject>();
            child->setColor(sfvg::Blues::DeepSkyBlue);
            float angle = 45.0f * getChildCount() * sfvg::DEG2RAD;
            child->setPosition(std::cos(angle)*100.0f, std::sin(angle)*100.0f);
        }

        if (Input::getKeyDown(Key::I)) {
            std::cout << "Object: " << (void*)getHandle().get() << std::endl;
            std::cout << "ShapeObject: " << (void*)getHandle().as<ShapeObject>().get() << std::endl;
            std::cout << "SquareObject: " << (void*)getHandle().as<SquareObject>().get() << std::endl;

            if (getHandle() == getHandle().as<ShapeObject>())
                print("Same!");
        }

        if (Input::getKeyDown(Key::M)) {
            auto ptr = detachChild(0);
            getChild(0)->attachChild(ptr);
        }

        if (Input::getKeyDown(Key::D) && getChildCount() > 0) {
            destroyChild(getChildCount() - 1);
        }

        if (Input::getKeyDown(Key::Num1))
            sendToFront();
        if (Input::getKeyDown(Key::Num2))
            sendToBack();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_square, states);
    }

    sfvg::SquareShape m_square;
};


// main
int main() {

    sfvg::sfvgInit();

    Engine engine;
    engine.setWindowSize(Vector2f(500, 500));
    engine.setBackgroundColor(sfvg::Whites::White);
    engine.setWindowTitle("Engine Testing");
    engine.setLayerCount(2);

    auto root = Object::make<PrimitiveObject<SquareShape>>();
    root->setRotation(45);
    root->setPosition(250, 250);
    root->setName("root");
    engine.setRoot(root);

    engine.run();

    return 0;
}

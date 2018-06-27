#include <SFVG/Tween.hpp>
#include <SFVG/Shape.hpp>
#include <SFVG/Animation.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <vector>
#include <tuple>

using namespace sfvg;

struct Rotate {
    template <typename TSubject>
    void apply(TSubject* subject) {
        subject->rotate(value);
    }
    float value;
};


struct Move {
    template <typename TSubject>
    void apply(TSubject* subject) {
        subject->move(value);
    }
    sf::Vector2f value;
};

int main(int argc, char* argv[]) {
    Shape shape;
    Animation<Rotate, Move> anim;
    anim.addKeyFrame().set<Move>(sf::Vector2f(2.0,2.0)).set<Rotate>(45.0f);
    anim.addKeyFrame().set<Move>(sf::Vector2f(3.0f, 3.0f));
    anim.applyKeyFrame(0, &shape);
    anim.applyKeyFrame(1, &shape);
    //anim.getKeyFrame(0).apply(&shape);
    //anim.getKeyFrame(1).apply(&shape);


    std::cout << shape.getPosition().x << " " << shape.getPosition().y << std::endl;
    std::cout << shape.getRotation() << std::endl;

    return 0;
}







// struct Property {
//     virtual void update() { }
//     void setSubject(void* subject) {
//         m_subject = subject;
//     }
//     void* m_subject;
// };

// template <typename T>
// struct Move : public Property {
//     Move(sf::Vector2f value) : m_value(value) {}
//     void update() override {
//         static_cast<T*>(m_subject)->move(m_value);
//     }
//     sf::Vector2f m_value;
// };

// template <typename T>
// struct Rotate : public Property {
//     Rotate(float value) : m_value(value) {}
//     void update() override {
//         static_cast<T*>(m_subject)->rotate(m_value);
//     }
//     float m_value;
// };

// template <typename T>
// class KeyFrame {
// public:
//     KeyFrame(T* subject) : m_subject(subject) {}

//     void addProperty(Property* property) {
//         property->setSubject(m_subject);
//         properties.push_back(property);
//     }

//     void update() {
//         for (std::size_t i = 0; i < properties.size(); ++i) {
//             properties[i]->update();
//         }
//     }

// private:
//     T* m_subject;
//     std::vector<Property*> properties;
// };

// int main(int argc, char* argv[]) {

//     sf::Sprite shape;

//     KeyFrame<sf::Sprite> key(&shape);
//     key.addProperty(new Move<sf::Sprite>(sf::Vector2f(2.0,2.0)));
//     key.addProperty(new Rotate<sf::Sprite>(45.0f));

//     key.update();

//     std::cout << shape.getPosition().x << " " << shape.getPosition().y << std::endl;
//     std::cout << shape.getRotation() << std::endl;

//     return 0;
// }

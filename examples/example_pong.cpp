#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Graphics.hpp>

using namespace sfvg;

//=============================================================================
// Player
//=============================================================================

class Player : public Object {
public:

    // Constructor
    Player() :
        m_rectangle(50,10),
        m_speed(200)
    {
        m_rectangle.setColor(Blues::DeepSkyBlue);
        setPosition(250, 450);
    }

    // Object Update
    void update() override {
        if (Input::getKey(Key::Left))
            move(-m_speed * Engine::deltaTime(), 0.0f);
        if (Input::getKey(Key::Right))
            move(m_speed * Engine::deltaTime(), 0.0f);
    }

    // Object Draw (called once per frame after update)
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_rectangle, states);
    }

private:
    RectangleShape m_rectangle;
    float m_speed;
};

//=============================================================================
// Ball
//=============================================================================

class Ball : public Object {
public:

    // Constructor
    Ball() :
        m_circle(10),
        m_speed(400)
    {
        m_circle.setColor(Reds::FireBrick);
        restart();
    }

    // Object Update
    void update() override {
        move(m_velocityVector * m_speed * Engine::deltaTime());
    }

    // Object Draw (called once per frame after update)
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_circle, states);
    }

    // restart ball
    void restart() {
        setPosition(250, 250);
        float vx = random(-0.5f,0.5f);
        float vy = random(-1.0f,1.0f);
        m_velocityVector = unit(Vector2f(vx,vy));
    }

    // reflect ball in X
    void reflectX() {
        m_velocityVector.x = -m_velocityVector.x;
    }

    // reflect ball in Y
    void reflectY() {
        m_velocityVector.y = -m_velocityVector.y;
    }

private:
    CircleShape m_circle;
    Vector2f m_velocityVector;
    float m_speed;
};


//=============================================================================
// PongGame
//=============================================================================

class PongGame : public Object {
public:

    // Constructor
    PongGame() {
        m_player = makeChild<Player>();
        m_ball = makeChild<Ball>();

        m_font.loadFromFile("../../fonts/Roboto-Bold.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(30);
        m_text.setPosition(10, 10);
        m_text.setFillColor(Whites::White);
    }

    // Object Update
    void update() override {

        // get position of player and ball
        auto playPos = m_player->getPosition();
        auto ballPos = m_ball->getPosition();

        // check player collision with walls
        if (playPos.x >= 475)
            m_player->setPosition(475, playPos.y);
        else if (playPos.x <= 25)
            m_player->setPosition(25, playPos.y);

        // check ball collision with walls
        if (ballPos.x <= 10 || ballPos.x >= 490)
            m_ball->reflectX();
        if (ballPos.y <= 10)
            m_ball->reflectY();

        // check ball collision with player
        if (ballPos.x - 10 < playPos.x + 25 &&
            ballPos.x + 10 > playPos.x - 25 &&
            ballPos.y - 10 < playPos.y + 5  &&
            ballPos.y + 10 > playPos.y - 5)
        {
            m_ball->reflectY();
            m_score++;
        }

        // check if ball lost
        if (ballPos.y > 510) {
            m_score = 0;
            m_ball->restart();
        }

        // update score text
        m_text.setString("Score: " + std::to_string(m_score));
    }

    // Object Draw (called once per frame after update)
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_text, states);
    }

private:
    Handle<Player> m_player;
    Handle<Ball> m_ball;

    int m_score;
    Font m_font;
    Text m_text;
};

//=============================================================================
// Main
//=============================================================================

int main()
{
    sfvg::sfvgInit();
    Engine engine;
    engine.setWindowSize(Vector2f(500,500));
    engine.setWindowTitle("Pong!");
    auto game = Object::make<PongGame>();
    game->setName("game");
    engine.setRoot(game);
    engine.run();
    return 0;
}

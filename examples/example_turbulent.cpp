#include <Carnot/Engine/Object.hpp>
#include <Carnot/Engine/Engine.hpp>
#include <Carnot/Engine/XboxController.hpp>
#include <Carnot/Graphics.hpp>
#include <chipmunk.h>

using namespace carnot;

//=============================================================================
// WORLD
//=============================================================================

#define WORLD_WIDTH  1800
#define WORLD_HEIGHT 1000

class World : public Object {
public:
    // Constructor
    World(Engine& engine) : Object(engine),  m_dt(1.0f/60.0f) {
        m_space = cpSpaceNew();
        cpSpaceSetIterations(m_space, 10);
        cpSpaceSetSleepTimeThreshold(m_space, 0.5f);
        cpSpaceSetCollisionSlop(m_space, 0.5f);
        enableGravity(true);
    }

    // Destructor
    ~World() {
        cpSpaceFree(m_space);
    }

    // Object Update
    void update() override {
        cpSpaceStep(m_space, m_dt);
        if (Input::getKeyDown(Key::G))
            enableGravity(!m_gravity);
    }

    cpSpace* space() {
        return m_space;
    }

    void enableGravity(bool enable) {
        if (enable)
            cpSpaceSetGravity(m_space, cpv(0,1000));
        else
            cpSpaceSetGravity(m_space, cpv(0,0));
        m_gravity = enable;
    }

private:
    cpSpace* m_space;
    float m_dt;
    bool m_gravity;
};

//=============================================================================
// PHYSICS OBJECT
//=============================================================================

class PhysicsObject : public Object {
public:

    // Constructor
    PhysicsObject(Handle<World> world, const Shape& shape, Color color) :
        m_world(world),
        m_gfx(shape),
        m_shape(nullptr),
        m_body(nullptr)
    {
        m_gfx.setColor(color);
    }

    // Destructor
    virtual ~PhysicsObject() {
        if (m_shape)
            cpShapeFree(m_shape);
        if (m_body)
            cpBodyFree(m_body);
    }

    // Update
    void update() override {
        auto position = cpBodyGetPosition(m_body);
        auto angle = cpBodyGetAngle(m_body);
        transform.setPosition(position.x, position.y);
        transform.setRotation(angle * RAD2DEG);
    }

    // Draw
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_gfx, states);
    }

    // Reposition
    virtual void reposition(float x, float y, float angle) = 0;

    void setColor(const Color& color) {
        m_gfx.setColor(color);
    }

protected:
    Handle<World> m_world;
    Shape m_gfx;
    cpShape* m_shape;
    cpBody* m_body;
};

//=============================================================================
// OBSTACLE
//=============================================================================

class Obstacle : public PhysicsObject {
public:
    // Constructor
    Obstacle(Handle<World> world, RectangleShape shape, Color color) :
        PhysicsObject(world, static_cast<Shape>(shape), color)
    {
        m_body = cpSpaceAddBody(m_world->space(), cpBodyNewStatic());
        m_shape = cpBoxShapeNew(m_body, shape.getWidth(), shape.getHeight(), 1.0);
        cpSpaceAddShape(m_world->space(), m_shape);
        cpShapeSetElasticity(m_shape, 0.0f);
        cpShapeSetFriction(m_shape, 0.0f);
        reposition(0, 0, 0);
    }

    // Reposition
    void reposition(float x, float y, float angle) override {
        cpBodySetPosition(m_body, cpv(x,y));
        cpBodySetAngle(m_body, angle * DEG2RAD);
        cpSpaceReindexShape(m_world->space(), m_shape);
    }
};

//=============================================================================
// PARTICLE
//=============================================================================

class Particle : public PhysicsObject {
public:

    // Constructor from RectangleShape
    Particle(Handle<World> world, RectangleShape shape, float mass, float elasticity, float friction, Color color) :
        PhysicsObject(world, static_cast<Shape>(shape), color)
    {
        auto moment = cpMomentForBox(mass, shape.getWidth(), shape.getHeight());
        m_body = cpSpaceAddBody(m_world->space(), cpBodyNew(mass, moment));
        m_shape = cpBoxShapeNew(m_body, shape.getWidth(), shape.getHeight(), 0.5f);
        cpSpaceAddShape(m_world->space(), m_shape);
        cpShapeSetElasticity(m_shape, elasticity);
        cpShapeSetFriction(m_shape, friction);
        reposition(0, 0, 0);
    }

    // Constructor from CircleShape
    Particle(Handle<World> world, CircleShape shape, float mass, float elasticity, float friction, Color color) :
        PhysicsObject(world, static_cast<Shape>(shape), color)
    {
        auto moment = cpMomentForCircle(mass, 0.0f, shape.getCircleRadius(), cpv(0,0));
        m_body = cpSpaceAddBody(m_world->space(), cpBodyNew(mass, moment));
        m_shape = cpCircleShapeNew(m_body, shape.getCircleRadius(), cpv(0,0));
        cpSpaceAddShape(m_world->space(), m_shape);
        cpShapeSetElasticity(m_shape, elasticity);
        cpShapeSetFriction(m_shape, friction);
        reposition(0, 0, 0);
    }

    // Reposition
    void reposition(float x, float y, float angle) override {
        cpBodySetPosition(m_body, cpv(x,y));
        cpBodySetAngle(m_body, angle * DEG2RAD);
    }

    Vector2f getPhysicsPosition() {
        auto pos = cpBodyGetPosition(m_body);
        return Vector2f(pos.x, pos.y);
    }

    // Apply Force
    void applyForceToCenter(float forceX, float forceY) {
        auto pos = cpBodyGetPosition(m_body);
        cpBodyApplyForceAtWorldPoint(m_body, cpv(forceX,forceY), cpv(pos.x,pos.y));
    }

    // Push Particle to Point
    void pushToPoint(Vector2f point, float force) {
        auto pos = cpBodyGetPosition(m_body);
        Vector2f v = unit(Vector2f(point.x - pos.x, point.y - pos.y));
        cpBodyApplyForceAtWorldPoint(m_body, cpv(v.x*force,v.y*force), cpv(pos.x,pos.y));
    }

    void torqueAboutPoint(Vector2f point, float force) {
        auto pos = cpBodyGetPosition(m_body);
        Vector2f v = unit(Vector2f(point.x - pos.x, point.y - pos.y));
        Vector2f n = normal(v);
        cpBodyApplyForceAtWorldPoint(m_body, cpv(n.x*force,n.y*force), cpv(pos.x,pos.y));
    }

    // Apply Torque
    void applyTorqueToCenter(float torque) {
        torque += cpBodyGetTorque(m_body);
        cpBodySetTorque(m_body, torque);
    }

};

//=============================================================================
// SWARM
//=============================================================================

class Scoreboard;

class Swarm : public Object {
public:
    Swarm(Handle<World> world, int ctrl, Color color, std::size_t particleCount) :
        m_world(world),
        m_particleCount(particleCount),
        m_xbox(ctrl),
        m_boost(0.5f),
        m_boostLossRate(0.5f),
        m_boostGainRate(0.1f)
    {
        RectangleShape shape(20,20);
        shape.setRadii(5.0f, 5);
        // CircleShape shape(10);
        for (std::size_t i = 0; i < m_particleCount; ++i) {
            auto particle = makeChild<Particle>(world, shape, 1.0f, 0.5f, 0.5f, color);
            particle->reposition(random(0, WORLD_WIDTH), random(0, WORLD_HEIGHT), random(0.0f, 90.0f));
            m_particles.push_back(particle);
        }

        if (m_xbox.isConnected()) {
            m_xbox.setDeadzone(XboxController::Axis::LX, 0.2f);
            m_xbox.setDeadzone(XboxController::Axis::LY, 0.2f);
            m_xbox.setDeadzone(XboxController::Axis::RX, 0.2f);
        }
        else
            print("Xbox Controller Not Connected!");

        RGB rgb{color.r,color.g,color.b,color.a};
        m_color = rgbToHsv(rgb);
    }

    void update() override {

        float fx   = m_xbox.getAxis(XboxController::LX) * 2000.0f;
        float fy   = -m_xbox.getAxis(XboxController::LY) * 2000.0f;

        float pull = (m_xbox.getAxis(XboxController::RT) - m_xbox.getAxis(XboxController::LT)) * 1000;
        float spin = -(m_xbox.getAxis(XboxController::RX)) * 100;

        updateSwarmCenter();

        m_boost += Engine::deltaTime() * m_boostGainRate * (1.0f - m_packingFactor) * 5.0f;

        if (m_xbox.getButton(XboxController::A)) {
            m_boost -= Engine::deltaTime() * m_boostLossRate;
            fx   += fx * 4.0f * m_boost;
            fy   += fy * 4.0f * m_boost;
            // spin += 4000.0f * m_boost;
            pull += pull * 2.0f * m_boost;
        }
        m_boost = clamp01(m_boost);

        updateColor();

        for (auto& particle : m_particles) {
            particle->applyForceToCenter(fx, fy);
            particle->pushToPoint(m_mean, pull);
            particle->torqueAboutPoint(m_mean, spin);
        }
    }

    // Get Swarm Center
    void updateSwarmCenter() {
        std::vector<float> xs, ys;
        xs.reserve(m_particleCount);
        ys.reserve(m_particleCount);
        for (auto& particle : m_particles) {
            auto particlePosition = particle->getPhysicsPosition();
            xs.push_back(particlePosition.x);
            ys.push_back(particlePosition.y);
        }
        m_stddev.x = stddev(xs, m_mean.x);
        m_stddev.y = stddev(ys, m_mean.y);
        m_packingFactor = clamp01(interp(magnitude(m_stddev), 100.0f, 2000.0f, 1.0f, 0.0f));

        float Ix  = 0.0f;
        float Iy  = 0.0f;
        float Ixy = 0.0f;
        for (int i = 0; i < xs.size(); ++i)
        {
            float rx = xs[i] - m_mean.x;
            float ry = ys[i] - m_mean.y;
            Ix += ry * ry;
            Iy += rx * rx;
            Ixy += rx * ry;
        }
        float Iavg = (Ix + Iy) * 0.5f;
        float R = std::sqrt(((Ix - Iy) * 0.5f)*((Ix - Iy) * 0.5f) + Ixy*Ixy);
        float L1 = Iavg + R;
        float L2 = Iavg - R;
        m_v1 = unit(Vector2f(-Ixy, -Ix + L1));
        m_v2 = unit(Vector2f(Iy - L2, Ixy));
    }

    // update Color
    void updateColor() {
        m_color.s = interp(m_boost, 0.0f, 1.0f, 0.5f, 1.0f);
        auto color = hsvToRgb(m_color);
        for (auto& particle : m_particles)
            particle->setColor(Color(color.r,color.g,color.b,color.a));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        // Stroke stroke1(2);
        // stroke1.setPoint(0, m_mean - m_v1 * 1000.0f);
        // stroke1.setPoint(1, m_mean + m_v1 * 1000.0f);
        // stroke1.setColor(Blues::DeepSkyBlue);
        // stroke1.setThicnkess(1.0f);

        // Stroke stroke2(2);
        // stroke2.setPoint(0, m_mean - m_v2 * 500.0f);
        // stroke2.setPoint(1, m_mean + m_v2 * 500.0f);
        // stroke2.setColor(Blues::DeepSkyBlue);
        // stroke2.setThicnkess(1.0f);

        // target.draw(stroke1, states);
        // target.draw(stroke2, states);
    }

private:

    friend class Scoreboard;
    Handle<World> m_world;
    std::size_t m_particleCount;
    std::vector<Handle<Particle>> m_particles;
    XboxController m_xbox;
    HSV m_color;
    Vector2f m_mean;
    Vector2f m_stddev;
    float m_packingFactor;
    float m_boost;
    float m_boostLossRate;
    float m_boostGainRate;
    Vector2f m_v1;
    Vector2f m_v2;
};

//=============================================================================
// BOUNDARY
//=============================================================================

class Boundary : public Object {
public:

    Boundary(Handle<World> world, float width, float height, float wallThickness) {
        RectangleShape vert(wallThickness, WORLD_HEIGHT + 2 * wallThickness);
        RectangleShape horz(WORLD_WIDTH + 2 * wallThickness, wallThickness);

        m_left = makeChild<Obstacle>(world, vert, Whites::White);
        m_left->reposition(-wallThickness/2, WORLD_HEIGHT/2, 0);
        m_right = makeChild<Obstacle>(world, vert, Whites::White);
        m_right->reposition(WORLD_WIDTH + wallThickness/2, WORLD_HEIGHT/2, 0);
        m_top = makeChild<Obstacle>(world, horz, Whites::White);
        m_top->reposition(WORLD_WIDTH/2, 0-wallThickness/2, 0);
        m_bottom = makeChild<Obstacle>(world, horz, Whites::White);
        m_bottom->reposition(WORLD_WIDTH/2, WORLD_HEIGHT + wallThickness/2, 0);
    }

private:
    Handle<Obstacle> m_left;
    Handle<Obstacle> m_right;
    Handle<Obstacle> m_top;
    Handle<Obstacle> m_bottom;
};

//=============================================================================
// SCOREBOARD
//=============================================================================

class Scoreboard : public Object {
public:

    Scoreboard(Handle<Swarm> swarm1, Handle<Swarm> swarm2) :
        m_swarm1(swarm1),
        m_swarm2(swarm2)
    {
        m_font.loadFromFile("../../fonts/Roboto-Bold.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(30);
        m_text.setFillColor(Whites::White);
        m_text.setPosition(10, 10);
        m_text.setString("Hello, World!");
    }

    void update() override {
        std::string s;
        s += "Swarm 1: " + numToStr(m_swarm1->m_boost) + "\n";
        s += "Swarm 2: " + numToStr(m_swarm2->m_boost);
        m_text.setString(s);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_text, states);
    }

private:
    Font m_font;
    Text m_text;

    Handle<Swarm> m_swarm1;
    Handle<Swarm> m_swarm2;
};

//=============================================================================
// MAIN
//=============================================================================

int main()
{
    Engine engine(WORLD_WIDTH, WORLD_HEIGHT);
    engine.window.setTitle("Physics Demo");
    engine.setLayerCount(2);

    // make world
    auto world = engine.makeRoot<World>();
    world->enableGravity(false);
    // make boundary
    world->makeChild<Boundary>(world, WORLD_WIDTH, WORLD_HEIGHT, 500);
    // make swarms
    auto swarm1 = world->makeChild<Swarm>(world, 0, Blues::DeepSkyBlue, 400);
    // auto swarm2 = world->makeChild<Swarm>(world, 1, Color(255,48,48), 400);

    //world->makeChild<Particle>(world, CircleShape(25.0f), 5.0f, 0.0f, 1.0f, Whites::White)->reposition(WORLD_WIDTH/2,WORLD_HEIGHT/2,0);
    world->makeChild<Particle>(world, CircleShape(25), 5.0f, 0.0f, 5.0f, Whites::White)->reposition(WORLD_WIDTH/2,WORLD_HEIGHT/2,0);
    // world->makeChild<Scoreboard>(swarm1, swarm2);

    // Run game
    engine.run();
    return 0;
}

// can separate blue and red using only one color acuation (momentum)
// try to separate one while the other holds you down

// absorb players
// dual blob mode
// elipse formation
// check outbound

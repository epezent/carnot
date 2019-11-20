#include <Physics/PhysicsSystem.hpp>
#include <Carnot/Glue/Box2D.inl>
#include <Engine/DebugSystem.hpp>
#include <Graphics/NamedColors.hpp>
#include <Utility/Print.hpp>
#include <Physics/Components/RigidBody.hpp>

namespace carnot {

//==============================================================================
// GLOBALS
//==============================================================================

class CollisionListener;

namespace {

class CarnotB2Draw;

float    g_dt;
float    g_scale;
float    g_invScale;

b2World* g_world;
CarnotB2Draw* g_draw;
CollisionListener* g_listener;

class CarnotB2Draw : public b2Draw {
public:
    	/// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override  {
        std::vector<Vector2f> v(vertexCount);
        for (std::size_t i = 0; i < vertexCount; ++i)
            v[i] = fromB2D(vertices[i]);
        Debug::drawPolygon(v, fromB2D(color), false);
    }

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override  {
        std::vector<Vector2f> v(vertexCount);
        for (std::size_t i = 0; i < vertexCount; ++i)
            v[i] = fromB2D(vertices[i]);
        Debug::drawPolygon(v, withAlpha(fromB2D(color),0.75f), true);
        Debug::drawPolygon(v, fromB2D(color), false);
    }

	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override  {
        Debug::drawCircle(fromB2D(center), g_invScale * radius, fromB2D(color), false);
    }

	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override  {
        Debug::drawCircle(fromB2D(center), g_invScale * radius, withAlpha(fromB2D(color),0.75f), true);
        Debug::drawCircle(fromB2D(center), g_invScale * radius, fromB2D(color), false);
    }

	/// Draw a particle array
	virtual void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) override  {
        if (colors) {
            for (int i = 0; i < count; ++i) {
                Debug::drawCircle(fromB2D(centers[i]), radius * g_invScale, fromB2D(colors[i].GetColor()), false);
            }
        }
        else {
            for (int i = 0; i < count; ++i) 
                Debug::drawCircle(fromB2D(centers[i]), radius * g_invScale, Whites::White, false);
        }
    }

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override  {
        Debug::drawLine(fromB2D(p1), fromB2D(p2), fromB2D(color));
    }

	/// Draw a transform. Choose your own length scale.
	virtual void DrawTransform(const b2Transform& xf) override  {
        auto p = fromB2D(xf.p);
        Debug::drawCircle(p, 5, Purples::Magenta);
        Debug::drawPoint(p + Vector2f(2,2), Purples::Magenta);
        Debug::drawPoint(p - Vector2f(2,2), Purples::Magenta);
    }

};

} // namespace

class CollisionListener : public b2ContactListener {
public:

    CollisionListener() : m_beginBuffer(), m_endBuffer() {
        m_beginBuffer.reserve(256);
        m_endBuffer.reserve(256);
    }

	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* contact) override {      
        RigidBody* rbA = static_cast<RigidBody*>(contact->GetFixtureA()->GetUserData());
        RigidBody* rbB = static_cast<RigidBody*>(contact->GetFixtureB()->GetUserData());        
        Collision colA = {rbB};
        Collision colB = {rbA};  
        b2WorldManifold man;
        contact->GetWorldManifold(&man);
        m_beginBuffer.push_back({rbA,colA});
        m_beginBuffer.push_back({rbB,colB});
    }

	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact) override {
        RigidBody* rbA = static_cast<RigidBody*>(contact->GetFixtureA()->GetUserData());
        RigidBody* rbB = static_cast<RigidBody*>(contact->GetFixtureB()->GetUserData());        
        Collision colA = {rbB};
        Collision colB = {rbA};        
        m_endBuffer.push_back({rbA,colA});
        m_endBuffer.push_back({rbB,colB});
    }

    void processCollisions() {
        for (auto& c : m_beginBuffer)
            c.first->onCollisionEnter.emit(c.second);
        m_beginBuffer.clear();
        for (auto& c : m_endBuffer) 
            c.first->onCollisionExit.emit(c.second);
        m_endBuffer.clear();
    }

    std::vector<std::pair<RigidBody*,Collision>> m_beginBuffer;
    std::vector<std::pair<RigidBody*,Collision>> m_endBuffer;

};

//==============================================================================
// USER API
//==============================================================================

namespace Physics {

void setDeltaTime(float dt) {
    g_dt = dt;
}

void setGravity(const Vector2f &g) {
    g_world->SetGravity(b2Vec2(g.x * g_scale, g.y * g_scale));
    for (auto body = g_world->GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true);
    }
}

Vector2f getGravity() {
    auto g = g_world->GetGravity();
    return Vector2f(g.x * g_invScale, g.y * g_invScale);
}

//==============================================================================
// DETAIL
//==============================================================================

namespace detail {

void init()
{
    g_dt       = 1.0f / 60.0f;
    g_scale    = 0.010f;
    g_invScale = 100.0f;
    g_world = new b2World(b2Vec2(0.0f, 981.0f * g_scale));
    g_draw  = new CarnotB2Draw();
    g_listener = new CollisionListener();
    				
    g_draw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit | b2Draw::e_particleBit);
    g_world->SetDebugDraw(g_draw);
    g_world->SetContactListener(g_listener);

}

void update() {
    static auto physicsID = Debug::gizmoId("Physics");
    g_world->Step(g_dt, 6, 2);
    g_listener->processCollisions();
    if (Debug::gizmoActive(physicsID))
        g_world->DrawDebugData();
}

void shutdown() {
    delete g_draw;
    delete g_world;
    delete g_listener;
}

PhysicsWorld* world() {
    return g_world;
}

float scale() {
    return g_scale;
}

float invScale() {
    return g_invScale;
}

} // namespace detail
} // namespace Physics
} // namespace carnot

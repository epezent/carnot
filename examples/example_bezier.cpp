#include <carnot>
using namespace carnot;

constexpr size_t SMOOTHNESS = 64;

template<int steps>
void bezier_table(Vector2f P[4], Vector2f results[steps + 1]) {
    static float C[(steps + 1) * 4], *K = 0;
    if (!K) {
        K = C;
        for (unsigned step = 0; step <= steps; ++step) {
            float t = (float) step / (float) steps;
            C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
            C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t;   // * P1
            C[step * 4 + 2] = 3 * (1 - t) * t*t;         // * P2
            C[step * 4 + 3] = t * t*t;                   // * P3
        }
    }
    for (unsigned step = 0; step <= steps; ++step) {
        ImVec2 point = {
            K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
            K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
        };
        results[step] = point;
    }
}

class Grab : public GameObject {
public:

    Grab(Vector2f pos, float radius, Color normal, Color hovered, Color grabbed) : 
        m_normal(normal), m_hovered(hovered), m_grabbed(grabbed), m_radius(radius)
    {
        auto circle = make<CircleShape>(radius,10);
        m_rend = addComponent<ShapeRenderer>(circle);
        m_rend->setColor(normal);
        transform.setPosition(pos);
    }

    void pos(Vector2f pos) { transform.setPosition(pos); }
    void pos(float x, float y) { transform.setPosition(x,y); }

    Vector2f pos() { return transform.getPosition(); }

    bool isGrabbed()  { return m_isGrabbed; }

    bool constrain(float left, float right, float top, float bottom) {
        bool constrained = false;
        if (pos().x <= left) {
            pos(left + Math::EPS, pos().y);
            constrained = true;
        }
        if (pos().x >= right) {
            pos(right - Math::EPS, pos().y);
            constrained = true;
        }
        if (pos().y <= top)
        {
            pos(pos().x, top);
            constrained = true; 
        }
        if (pos().y >= bottom) {
            pos(pos().x, bottom);
            constrained = true;
        }
        return constrained;
    }


    bool grabBehavior() {
        if (m_isGrabbed) {
            transform.move(Input::dragDelta(MouseButton::Left));
            if (Input::dragEnded(MouseButton::Left)) {
                m_isGrabbed = false;
                m_rend->setColor(m_hovered);
                return false;
            }
            return true;
        }
        else {
            auto d = Math::magnitude(pos() - Input::getMousePosition());

            if (d < 5) {
                if (Input::dragStarted(MouseButton::Left)) {
                    m_isGrabbed = true;
                    m_rend->setColor(m_grabbed);
                    return true;
                }
                else {
                    m_rend->setColor(m_hovered);
                    return false;
                }
            }
            else {
                m_rend->setColor(m_normal);
                return false;
            }
        }
    }

private:
    Handle<ShapeRenderer> m_rend;
    bool m_isGrabbed = false;
    Color m_normal, m_hovered, m_grabbed;
    float m_radius;
};

class GrabGroup : public GameObject {
public:
    Handle<Grab> p,l,r;

    Handle<LineRenderer> lr;

    void start() {
        lr = addComponent<LineRenderer>(3);
        lr->setColor(Grays::Gray20);
    }

    void update() {
        l->setEnabled(prev);
        r->setEnabled(next);
        if (prev)
            lr->setPoint(0, l->pos());
        else        
            lr->setPoint(0, p->pos());            
        lr->setPoint(1, p->pos());
        if (next) 
            lr->setPoint(2, r->pos());
        else
            lr->setPoint(2, p->pos());           

    }

    GrabGroup(Vector2f pos) {
        p = makeChild<Grab>(pos, 3, Greens::Chartreuse, Blues::DeepSkyBlue, Reds::FireBrick);
        l = makeChild<Grab>(pos - Vector2f(20,0), 2, Grays::Gray30, Blues::DeepSkyBlue, Reds::FireBrick);
        r = makeChild<Grab>(pos + Vector2f(20,0), 2, Grays::Gray30, Blues::DeepSkyBlue, Reds::FireBrick);
    }

    void insert(Handle<GrabGroup> g) {
        if (prev) {
            prev->next = g;
            g->prev = prev;
        }
        g->next = getHandle();
        prev = g;
    }

    void push(Handle<GrabGroup> g) {
        next = g;
        g->prev = getHandle();
    }

    void constrain(FloatRect rect) {
        p->constrain(prev ? prev->p->pos().x : rect.left, next ? (next->p->isGrabbed() ? p->pos().x : next->p->pos().x) : rect.left + rect.width, rect.top, rect.top + rect.height);
        l->constrain(prev ? prev->p->pos().x : -Math::INF, p->pos().x                                                                           , rect.top - rect.height/4, rect.top + 1.25 * rect.height);
        r->constrain(p->pos().x, next ? next->p->pos().x : Math::INF                                                                            , rect.top - rect.height/4, rect.top + 1.25 * rect.height);
    }

    bool dragBehavior() {
        if (l->grabBehavior()) {
            return true;
        }
        else if (r->grabBehavior()) {
            return true;
        }
        else if  (p->grabBehavior()) {
            l->transform.move(Input::dragDelta(MouseButton::Left));
            r->transform.move(Input::dragDelta(MouseButton::Left));
            return true;
        }
        else
            return false;
    }

    Handle<GrabGroup> prev, next;
};

class PolyBezier : public GameObject {
public:
    std::vector<Handle<GrabGroup>> groups;

    Handle<GrabGroup> m_root;
    Handle<GrabGroup> m_head;

    Handle<LineRenderer> bezier;

    FloatRect m_rect;

    PolyBezier(FloatRect rect) : m_rect(rect) { 
        bezier = addComponent<LineRenderer>();
        bezier->setColor(Greens::Chartreuse);
        bezier->setLayer(2);
        auto boundsGo = makeChild<GameObject>();
        auto bounds = boundsGo->addComponent<ShapeRenderer>(make<RectangleShape>(rect.width, rect.height));
        boundsGo->transform.setPosition(rect.getPosition() + 0.5f * rect.getSize());
        bounds->setColor(Grays::Gray10);
    }

    void update() {
        // get mouse pos
        auto mouse = Input::getMousePosition();

        // add point if right-click
        if (Input::getMouseDown(MouseButton::Right) && !pointExistsAt(mouse))
        {
            auto newG = makeChild<GrabGroup>(mouse);
            if (!m_root) {
                m_root = newG;
                m_head = m_root;
            }
            else
            {
                bool inserted = false;
                for (auto g = m_root; g; g = g->next)
                {
                    if (g->p->pos().x > newG->p->pos().x) {
                        g->insert(newG);
                        if (g == m_root)
                            m_root = newG;
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    m_head->push(newG);
                    m_head = newG;
                }
            }            
        }
        // drag updates
        for (auto g = m_root; g; g = g->next) {
            if (g->dragBehavior())
                break;
        }
        for (auto g = m_root; g; g = g->next) {
            g->constrain(m_rect);
        }
        
        // render lines
        int i = 0;
        for (auto g = m_root; g; g = g->next) {
            i++;
        }
        
        if (i > 1) {
            bezier->setPointCount((i-1) * (SMOOTHNESS +1));
            auto g = m_root;
            int k = 0;
            for (int j = 0; j < i -1; ++j) {
                Vector2f P[4] = {g->p->pos(), g->r->pos(), g->next->l->pos(), g->next->p->pos()};
                bezier_table<SMOOTHNESS>(P, results);
                for (int l = 0; l < SMOOTHNESS + 1; ++l) {
                    bezier->setPoint(k++, results[l]);
                }
                g = g->next;
            }
        }
        
    }

    bool pointExistsAt(Vector2f point) {
        // ...
        return false;
    }

    Vector2f results[SMOOTHNESS + 1];


};



int main(int argc, char const *argv[])
{
    Engine::init(1000,500,"Bezier Demo");
    Engine::makeRoot<PolyBezier>(FloatRect(100,150,800,200));
    Engine::setLayerCount(3);
    Engine::run();
    return 0;
}


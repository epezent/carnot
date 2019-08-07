#include <carnot>

using namespace carnot;

class IkJoint : public GameObject {
public:

    Ptr<CircleShape> shape;
    Handle<ShapeRenderer> sr;

    IkJoint() {
        shape = make<CircleShape>(5.0f);
        sr = addComponent<ShapeRenderer>(shape);
        sr->setColor(Color::White);
        sr->setLayer(1);
    }
};

class IkSolver : public GameObject {
public:

    int l = 40;
    int n = 4;
    std::vector<Handle<IkJoint>> J;
    std::vector<Vector2f> P = std::vector<Vector2f>(n);
    std::vector<float> D = std::vector<float>(n-1, l);
    Handle<LineRenderer> stroke;
    Vector2f t = Vector2f(200,200);
    bool animate = false;

    Handle<GameObject> start;
    Handle<GameObject> target;

    IkSolver() {
        for (auto& i : range(0,n))
            J.push_back(makeChild<IkJoint>());
        stroke = addComponent<LineRenderer>(n);
        stroke->setColor(Whites::White);
        // stroke->setThickness(1);
        start = makeChild<GameObject>();
        target = makeChild<GameObject>();
        auto sr = start->addComponent<ShapeRenderer>(make<CircleShape>(8.0f));
        sr->setColor(Blues::DeepSkyBlue);
        sr->setLayer(0);
        sr = target->addComponent<ShapeRenderer>(make<CircleShape>(8.0f));
        sr->setColor(Reds::FireBrick);
        sr->setLayer(0);
    }

    void resize() {
        for (auto& j : J)
            j->destroy();
        J.clear();
        for (auto& i : range(0,n))
            J.push_back(makeChild<IkJoint>());
        stroke->setPointCount(n);
        P.resize(n);
        D.resize(n);
        for (auto& d : D)
            d = (float)l;
    }

    void update() {

        ImGui::Begin("FABRIK Demo", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::DragInt("n", &n,1.0f,1,200)) 
            resize();
        if (ImGui::DragInt("l",&l,1.0f,1,200)) 
            resize();
        ImGui::Checkbox("Animate",&animate);
        ImGui::End();

        if (animate) {
            if (Input::getMouseDown(MouseButton::Left) && !animating) {
                t = Input::getMousePosition();
                startCoroutine(fabrikCoro());
            }      
            if (Input::getMouseDown(MouseButton::Right) && !animating) {
                P[0] = Input::getMousePosition();
                startCoroutine(fabrikCoro());
            }          
        }
        else {
            if (Input::getMouse(MouseButton::Left))
                t = Input::getMousePosition();
            if (Input::getMouse(MouseButton::Right))
                P[0] = Input::getMousePosition();
            fabrik();
        }

        for (int i = 0; i < J.size(); ++i) {
            J[i]->transform.setPosition(P[i]);
            stroke->setPoint(i, P[i]);
        }
        start->transform.setPosition(P[0]);
        target->transform.setPosition(t);
    }


    void fabrik() {
        auto nn = P.size() - 1;
        float dist = Math::magnitude(t-P[0]);
        if (dist > Math::sum(D)) {
            for (auto i = 0; i < nn; ++i) {
                auto r = Math::magnitude(t - P[i]);
                auto lambda = D[i] / r;
                P[i+1] = (1 - lambda) * P[i] + lambda * t;
            }
        }
        else {
            auto b = P[0];
            auto difA = Math::magnitude(P[nn]-t);
            while (difA > 0.001f) {
                // STAGE 1: FORWARD REACHING
                P[nn] = t;
                for (auto i = nn - 1; i != -1; i--) {
                    auto r = Math::magnitude(P[i+1] - P[i]);
                    auto lambda = D[i] / r;
                    P[i] = (1 - lambda) * P[i+1] + lambda * P[i];
                }
                // STAGE 2: BACKWARD REACHING
                P[0] = b;
                for (auto i = 0; i < nn - 1; ++i) {
                    auto r = Math::magnitude(P[i+1] - P[i]);
                    auto lambda = D[i] / r;
                    P[i+1] = (1-lambda) * P[i] + lambda * P[i+1];                
                }
                difA = Math::magnitude(P[nn]-t);
            }
        }
    }

    bool animating = false;

    Enumerator fabrikCoro() {
        animating = true;
        auto nn = P.size() - 1;
        float dist = Math::magnitude(t-P[0]);
        if (dist > Math::sum(D)) {
            for (auto i = 0; i < nn; ++i) {
                auto r = Math::magnitude(t - P[i]);
                auto lambda = D[i] / r;
                P[i+1] = (1 - lambda) * P[i] + lambda * t;
                co_yield new WaitForSeconds(0.05f);
            }
        }
        else {
            auto b = P[0];
            auto difA = Math::magnitude(P[nn]-t);
            while (difA > 0.001f) {
                // STAGE 1: FORWARD REACHING
                P[nn] = t;
                for (auto i = nn - 1; i != -1; i--) {
                    auto r = Math::magnitude(P[i+1] - P[i]);
                    auto lambda = D[i] / r;
                    P[i] = (1 - lambda) * P[i+1] + lambda * P[i];
                    co_yield new WaitForSeconds(0.05f);
                }
                // STAGE 2: BACKWARD REACHING
                P[0] = b;
                for (auto i = 0; i < nn - 1; ++i) {
                    auto r = Math::magnitude(P[i+1] - P[i]);
                    auto lambda = D[i] / r;
                    P[i+1] = (1-lambda) * P[i] + lambda * P[i+1];   
                    co_yield new WaitForSeconds(0.05f);             
                }
                difA = Math::magnitude(P[nn]-t);
            }
        }
        animating = false;
    }

};



int main(int argc, char const *argv[])
{
    Engine::init(800,800, "FABRIK Demo");
    Engine::setLayerCount(2);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<IkSolver>();
    Engine::run();
    return 0;
}

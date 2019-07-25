#include <carnot>

using namespace carnot;

class IkJoint : public GameObject {
public:

    Ptr<CircleShape> shape;
    Handle<ShapeRenderer> sr;

    IkJoint() {
        shape = make<CircleShape>(2.0f);
        sr = addComponent<ShapeRenderer>(shape);
        sr->setColor(Color::White);
    }
};

static void fabrik(Vector2f t, std::vector<Vector2f>& P, const std::vector<float>& D) {
    auto n = P.size() - 1;
    float dist = Math::magnitude(t-P[0]);
    if (dist > Math::sum(D)) {
        for (auto i = 0; i < n; ++i) {
            auto r = Math::magnitude(t - P[i]);
            auto lambda = D[i] / r;
            P[i+1] = (1 - lambda) * P[i] + lambda * t;
        }
    }
    else {
        auto b = P[0];
        auto difA = Math::magnitude(P[n]-t);
        for (int k = 0; k < 20; k++) {
            // STAGE 1: FORWARD REACHING
            P[n] = t;
            for (auto i = n - 1; i != -1; i--) {
                auto r = Math::magnitude(P[i+1] - P[i]);
                auto lambda = D[i] / r;
                P[i] = (1 - lambda) * P[i+1] + lambda * P[i];
            }
            // STAGE 2: BACKWARD REACHING
            P[0] = b;
            for (auto i = 0; i < n - 1; ++i) {
                auto r = Math::magnitude(P[i+1] - P[i]);
                auto lambda = D[i] / r;
                P[i+1] = (1-lambda) * P[i] + lambda * P[i+1];                
            }
            difA = Math::magnitude(P[n]-t);
        }
    }
}

class IkSolver : public GameObject {
public:

    int n = 200;
    std::vector<Handle<IkJoint>> J;
    std::vector<Vector2f> P = std::vector<Vector2f>(n);
    std::vector<float> Q = std::vector<float>(n-1, 0);
    std::vector<float> D = std::vector<float>(n-1, 5);
    Handle<LineRenderer> stroke;
    Vector2f t = Vector2f(200,200);

    IkSolver() {
        for (auto& i : range(0,n))
            J.push_back(makeChild<IkJoint>());

        stroke = addComponent<LineRenderer>(n);
        stroke->setColor(Whites::White);
        // stroke->setThickness(1);
    }

    void update() {

        if (Input::getMouse(MouseButton::Left))
            t = Input::getMousePosition();

        if (Input::getMouse(MouseButton::Right))
            P[0] = Input::getMousePosition();

        fabrik(t,P,D);

        for (int i = 0; i < n; ++i) {
            J[i]->transform.setPosition(P[i]);
            stroke->setPoint(i, P[i]);
        }
    }

    bool useInverse;
    float x, y;

};



int main(int argc, char const *argv[])
{
    Engine::init(800,800);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<IkSolver>();
    Engine::run();
    return 0;
}

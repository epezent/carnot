#include <carnot>
#include <random>


using namespace carnot;

class Dots : public GameObject {
public:
    Dots() : dist(0,1) {};

    void start() {
        generateDots({250,250}, 50, 3000);
        generateDots({500,250}, 50, 3000);
        generateDots({750,250}, 50, 3000);
    }

    void update() {
        for (auto& group : dotGroups) {
            for (auto& dot : group) {
                ImGui::GetForegroundDrawList()->AddCircleFilled(dot, 2, ImGui::GetColorU32(Grays::Gray50), 4);
            }
        }
    }

    void generateDots(Vector2f center, float radius, int n) {
        std::vector<Vector2f> dots;
        dots.reserve(n);
        for (int i = 0; i < n; ++i) {
            auto loc = center + Vector2f(dist(gen) * radius, dist(gen) * radius);
            dots.push_back(loc);
        }
        dotGroups.push_back(dots);
    }

    std::vector<std::vector<Vector2f>> dotGroups;
    std::default_random_engine gen;
    std::normal_distribution<float> dist;
};


int main(int argc, char const *argv[])
{
    Engine::init(1500,500,"Dots");
    Engine::makeRoot<Dots>();
    Engine::run();
    return 0;
}

#include <carnot>

using namespace carnot;

// Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// Any live cell with more than three live neighbors dies, as if by overpopulation.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Sequence<Color> g_colorSpectrum;

class Cell : public GameObject {
public:

    Cell(float size) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(Whites::White);
        sr->setShape(make<SquareShape>(size));
        sr->setEnabled(false);
    }

    void update() override {
        auto n = livingNeighbors();
        if (alive) {
            if (n < 2 || n > 3)  willLive = false;
            else                 willLive = true;
        }
        else {
            if (n == 3)          willLive = true;
        }
    }

    void lateUpdate() override {
        if (willLive) {
            alive = true;
            age++;
        }
        else {
            alive = false;
            age = 0;
        }
        if (alive)
            sr->setEnabled(true);
        else
            sr->setEnabled(false);
        float t = Math::clamp01((float)age / (500.0f));
        auto color = g_colorSpectrum(t);
        sr->setColor(color);
    }

    std::size_t livingNeighbors() {
        std::size_t i = 0;
        i += top->left->alive ? 1 : 0;
        i += top->alive ? 1 : 0;
        i += top->right->alive ? 1 :0;
        i += left->alive ? 1 : 0;
        i += right->alive ? 1 : 0;
        i += bottom->left->alive ? 1 : 0;
        i += bottom->alive ? 1 : 0;
        i += bottom->right->alive ? 1 : 0;
        return i;
    }

    int age = 0;
    bool alive    = false;
    bool willLive = false;
    Handle<Cell> left, right, top, bottom;
    Handle<ShapeRenderer> sr;
};

class Conway : public GameObject {
public:
    Conway(float _size) :
        width(Engine::getView(0).getSize().x), height(Engine::getView(0).getSize().y), size(_size),
        R((std::size_t)(height/size)),
        C((std::size_t)(width/size))
    {
        cells.resize(R);
        loadBg = addComponent<ShapeRenderer>(make<RectangleShape>(500,20));
        loadBg->setColor(Grays::Gray50);
        loadBg->getShape()->move(960,540);
        loadRect = make<RectangleShape>(0,20);
        loadFg = addComponent<ShapeRenderer>(loadRect);
        loadFg->setColor(Whites::White);
        loadFg->getShape()->move(960,540);
        startCoroutine(load());
    }

    Enumerator load() {
        for (auto r : range(R)) {
            cells[r].resize(C);
            for (auto c : range(C)) {
                auto cell = makeChild<Cell>(size);
                cell->setEnabled(false);
                cell->transform.setPosition(size/2 + c * size, size/2 + r * size);

                // setup doubly link list
                if (r != 0) {
                    cell->top  = cells[r-1][c];
                    cell->top->bottom = cell;
                    if (r == R-1) {
                        cell->bottom = cells[0][c];
                        cell->bottom->top = cell;
                    }
                }
                if (c != 0) {
                    cell->left = cells[r][c-1];
                    cell->left->right = cell;
                    if (c == C-1) {
                        cell->right = cells[r][0];
                        cell->right->left = cell;
                    }
                }
                cells[r][c] = cell;
            }
            if (r%2) {
                float percentDone = (float)r/(float)R;
                loadRect->setWidth(500 * percentDone);
                co_yield nullptr;
            }
        }
        for (auto r : range(R))
            for(auto c : range(C))
                cells[r][c]->setEnabled(true);
        loaded = true;
        loadBg->setEnabled(false);
        loadFg->setEnabled(false);
        spawn();
    }

    void gliderBR(std::size_t r, std::size_t c) {
        auto cell = cells[r][c];
        cell->top->left->alive     = false;
        cell->top->alive           = true;
        cell->top->right->alive    = false;
        cell->left->alive          = false;
        cell->alive                = false;
        cell->right->alive         = true;
        cell->bottom->left->alive  = true;
        cell->bottom->alive        = true;
        cell->bottom->right->alive = true;
    }

    void gliderBL(std::size_t r, std::size_t c) {
        auto cell = cells[r][c];
        cell->top->left->alive     = false;
        cell->top->alive           = true;
        cell->top->right->alive    = false;
        cell->left->alive          = true;
        cell->alive                = false;
        cell->right->alive         = false;
        cell->bottom->left->alive  = true;
        cell->bottom->alive        = true;
        cell->bottom->right->alive = true;
    }

    void spawn() {
        for (auto i : range(500)) {
            gliderBR(Random::range(0,R-1),Random::range(0,C-1));
            gliderBL(Random::range(0,R-1),Random::range(0,C-1));
        }
    }

    void update() override {
        if (loaded && Input::getKeyDown(Key::Space)) {
            spawn();
        }
    }

public:
    bool loaded = false;
    Handle<ShapeRenderer> loadBg, loadFg;
    Ptr<RectangleShape> loadRect;
    float width, height, size;
    std::size_t R, C;
    std::vector<std::vector<Handle<Cell>>> cells;
};

int main(int argc, char const *argv[])
{
    g_colorSpectrum[0.0f] = Whites::White;
    g_colorSpectrum[0.2f] = Yellows::PaleGoldenrod;
    g_colorSpectrum[0.4f] = Reds::LightSalmon;
    g_colorSpectrum[0.6f] = Reds::LightCoral;
    g_colorSpectrum[0.8f] = Purples::Plum;
    g_colorSpectrum[1.0f] = Grays::LightSlateGray;

    Engine::init();
    Engine::makeRoot<Conway>(5);
    Engine::run();
    return 0;
}

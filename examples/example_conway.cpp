#include <SFVG/SFVG.hpp>

using namespace sfvg;

// Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// Any live cell with more than three live neighbors dies, as if by overpopulation.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

class Cell : public GameObject {
public:

    Cell(float size) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(Whites::White);
        sr->shape = make<SquareShape>(size);
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
        if (willLive)
            alive = true;
        else
            alive = false;
        if (alive)
            sr->setEnabled(true);
        else
            sr->setEnabled(false);
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

    bool alive    = false;
    bool willLive = false;
    Handle<Cell> left, right, top, bottom;
    Handle<ShapeRenderer> sr;
};

class God : public GameObject {
public:
    God(float _width, float _height, float _size) :
        width(_width), height(_height), size(_size)
    {
        auto R = (std::size_t)(height/size);
        auto C = (std::size_t)(width/size);
        cells.resize(R);
        for (auto r : range(R)) {
            cells[r].resize(C);
            for (auto c : range(C)) {
                auto cell = makeChild<Cell>(size);
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
        }

        for (auto i : range(100)) {
            gliderBR(random(0,R-1),random(0,C-1));
            gliderBL(random(0,R-1),random(0,C-1));
        }

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


public:
    float width, height, size;
    std::vector<std::vector<Handle<Cell>>> cells;
};

int main(int argc, char const *argv[])
{
    Engine::init();
    Engine::window->setFramerateLimit(10);
    Engine::makeRoot<God>(1920,1080,10);
    Engine::run();
    return 0;
}

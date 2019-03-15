// #define EE_USE_DISCRETE_GPU
// #define EE_NO_CONSOLE

# include <EE/EE.hpp>
#include <deque>

using namespace ee;

//==============================================================================
// BASIC MATRIX OPERATIONS
//==============================================================================

typedef std::vector<std::vector<char>> Matrix;

inline Matrix ones(std::size_t r, std::size_t c)
{
    return Matrix(r, std::vector<char>(c, 1));
}

inline Matrix zeros(std::size_t r, std::size_t c)
{
    return Matrix(r, std::vector<char>(c, 0));
}

inline void flipLR(Matrix &mat)
{
    for (auto &row : mat)
        std::reverse(row.begin(), row.end());
}

inline void flipUD(Matrix &mat)
{
    std::reverse(mat.begin(), mat.end());
}

bool allZeros(const Matrix &mat)
{
    for (auto &row : mat)
        for (auto &col : row)
            if (col != 0)
                return false;
    return true;
}

std::size_t size(const Matrix& mat, int dim) {
    if (dim == 0)
        return mat.size();
    else if (dim == 1)
        return mat[0].size();
    return 0;
} 

//==============================================================================
// GLOBAL DEFINITIONS
//==============================================================================

const float g_sideLength = 30.0f;
const float g_gridSize   = (2.0f + sqrt(2.0f)) * 0.5f * g_sideLength * 0.999f;

const std::size_t g_numPieces = 14;

const Matrix g_matBoard {
    {0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 4, 8, 4, 8, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 4, 8, 4, 8, 4, 8, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 8, 4, 8, 4, 8, 4, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 4, 8, 4, 8, 4, 8, 4, 8, 0, 0},
    {0, 0, 0, 0, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 0},
    {0, 0, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8},
    {0, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 0},
    {0, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 0, 0},
    {8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 0, 0, 0},
    {0, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 0, 0, 0, 0},
    {0, 0, 0, 4, 8, 4, 8, 4, 8, 4, 8, 0, 0, 0, 0, 0},
    {0, 0, 0, 8, 4, 8, 4, 8, 4, 8, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 8, 4, 8, 4, 8, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const std::vector<Matrix> g_matPieces
{
    {{8, 4, 8, 4}, {0, 8, 4, 8}},                                           // RED        {9 0} 
    {{0, 4, 8}, {4, 8, 4}, {8, 4, 8}},                                      // ORANGE     {6 1} 
    {{8, 4, 0}, {4, 8, 4}, {8, 4, 0}},                                      // GOLD       {5 4} 
    {{0, 4, 0}, {4, 8, 4}, {8, 4, 8}, {4, 8, 4}, {8, 4, 0}},                // YELLOW     {1 6} 
    {{4, 8, 0, 0}, {8, 4, 8, 0}, {0, 8, 4, 8}, {0, 4, 8, 4}, {0, 8, 4, 0}}, // CHARTREUSE {0 8} 
    {{4, 8, 0}, {8, 4, 8}, {0, 8, 4}},                                      // LIME       {11 3}
    {{4, 0, 0, 0}, {8, 4, 0, 0}, {4, 8, 4, 0}, {0, 4, 8, 4}},               // GREEN      {8 4} 
    {{0, 0, 0, 8, 4}, {0, 0, 8, 4, 8}, {0, 8, 4, 8, 0}, {8, 4, 0, 0, 0}},   // TURQUOISE  {5 5} 
    {{0, 0, 0, 4, 0}, {0, 8, 4, 8, 4}, {8, 4, 8, 4, 8}, {0, 8, 4, 8, 4}},   // AQUA       {6 7} 
    {{0, 8, 4, 8}, {8, 4, 8, 0}, {4, 8, 4, 0}, {8, 0, 0, 0}},               // DODGERBLUE {4 10}
    {{0, 4, 8}, {0, 8, 4}, {8, 4, 0}, {0, 8, 0}},                           // BLUE       {12 5}
    {{8, 4, 8}, {4, 8, 0}, {8, 0, 0}},                                      // VIOLET     {11 8}
    {{4, 8, 0}, {8, 4, 8}, {4, 8, 0}, {8, 0, 0}},                           // PURPLE     {7 11}
    {{4, 8, 0}, {8, 4, 8}, {4, 8, 0}},                                      // MAGENTA    {5 13}
};

const std::vector<Vector2i> g_solutions {
    {9, 0}, 
    {6, 1}, 
    {5, 4}, 
    {1, 6}, 
    {0, 8}, 
    {11, 3},
    {8, 4}, 
    {5, 5}, 
    {7, 6}, 
    {4, 10},
    {12, 5},
    {11, 8},
    {7, 11},
    {5, 13},
};

const std::vector<Color> g_colors {
    Reds::Red,
    Oranges::Orange,
    Yellows::Gold,
    Yellows::Yellow,
    Greens::Chartreuse,
    Greens::Lime,
    Greens::Green,
    Cyans::Turquoise,
    Cyans::Aqua,
    Blues::DeepSkyBlue,
    Blues::Blue,
    Purples::Violet,
    Purples::Purple,
    Purples::Magenta
};

//==============================================================================
// HELPERS
//==============================================================================

Vector2f coordPosition(const Vector2i& coord) {
    return Vector2f(coord.y * g_gridSize, coord.x * g_gridSize);
}

Vector2f coordPosition(int i, int j) {
    return coordPosition(Vector2i(i,j));
}

//==============================================================================
// GRID GIZMO
//==============================================================================

class GridGizmo : public Component {
public:
    GridGizmo(GameObject& go) : Component(go) { }
    void onGizmo() override {
        static auto id = Debug::gizmoId("Grid");
        if (Debug::gizmoActive(id)) {
            for (int i = 0; i < 16; ++i) {
                for (int j = 0; j < 16; ++j)
                    Debug::drawText(str(i)+","+str(j), coordPosition(Vector2i(i,j)), Whites::White);
                Debug::drawLine(coordPosition(-1, i), coordPosition(16, i), Debug::gizmoColor(id));
                Debug::drawLine(coordPosition(i, -1), coordPosition(i, 16), Debug::gizmoColor(id));
            }
        }
    }
};

//==============================================================================
// BOARD
//==============================================================================

class Board : public GameObject
{
  public:

    Handle<ShapeRenderer> bg;

    Board() : GameObject("board"), matrix(g_matBoard), color(Grays::Gray80)
    {
        addComponent<GridGizmo>();

        bg = addComponent<ShapeRenderer>();
        bg->shape->setPointCount(4);
        bg->shape->setPoint(0, coordPosition(9,-2));
        bg->shape->setPoint(1, coordPosition(-2,9));
        bg->shape->setPoint(2, coordPosition(6,17));
        bg->shape->setPoint(3, coordPosition(17,6));

        auto alpha = color;;
        alpha.a = 128;
        bg->setGradient(Gradient(color,alpha, 45.0f));
        bg->shape->setRadii(g_gridSize);
        bg->shape->setHoleCount(1);
    }

    Enumerator makeShape()
    {
        auto sqr = SquareShape(g_sideLength);
        auto oct = PolygonShape(8, PolygonShape::SideLength, g_sideLength);
        oct.rotate(360.0f / 16.0f);
        oct.applyTransform();
        auto R = matrix.size();
        auto C = matrix[0].size();
        std::deque<Shape> shapes;
        for (std::size_t r = 0; r < R; ++r)
        {
            for (std::size_t c = 0; c < C; ++c)
            {
                if (matrix[r][c] == 4)
                    shapes.push_back(sqr);
                else if (matrix[r][c] == 8)
                    shapes.push_back(oct);
                oct.move(g_gridSize, 0);
                oct.applyTransform();
                sqr.move(g_gridSize, 0);
                sqr.applyTransform();
            }
            oct.move(-g_gridSize * C, g_gridSize);
            oct.applyTransform();
            sqr.move(-g_gridSize * C, g_gridSize);
            sqr.applyTransform();
        }
        Shape shape;
        while (!shapes.empty())
        {
            auto toMerge = shapes.front();
            shapes.pop_front();
            auto merged = Shape::clipShapes(shape, toMerge, Shape::Union);
            if (merged.size() == 1)
            {
                shape = merged[0];
                bg->shape->setHole(0, shape);
                co_yield nullptr;
            }
            else
                shapes.push_back(toMerge);
        }
        shape = Shape::offsetShape(shape, 2.0f);
        bg->shape->setHole(0, shape);
    }

    void update()
    {
        if (Input::getKeyDown(Key::B))
            startCoroutine(makeShape());
    }

    Matrix matrix;
    Color color;
};

//==============================================================================
// PIECE
//==============================================================================

class Piece : public GameObject
{
  public:
    Handle<ShapeRenderer> sr;

    Piece(const Matrix &mat, const Color &col) : matrix(mat),
                                                 color(col)
    {
        sr = addComponent<ShapeRenderer>();
        auto alpha = color;;
        alpha.a = 128;
        sr->setGradient(Gradient(alpha,color,45.0f));
    }

    void update()
    {
        auto localPos = transform.worldToLocal(Input::getMousePosition());
        if (inBounds(localPos, sr->getLocalBounds()) &&  sr->shape->isInside(localPos))
        {
            sr->setColor(Whites::White);
            auto scale = transform.getScale();
            if (Input::getKeyDown(Key::Left) || Input::getKeyDown(Key::Right))
                transform.setScale(-1 * scale.x, scale.y);
            else if (Input::getKeyDown(Key::Up) || Input::getKeyDown(Key::Down))
                transform.setScale(scale.x, -1 * scale.y);
        }
        else
        {
            auto alpha = color;;
            alpha.a = 128;
            sr->setGradient(Gradient(color,alpha,45.0f));
        }

        if (Input::getKeyDown(Key::P))
            startCoroutine(makeShape());
    }

    Enumerator makeShape()
    {
        auto sqr = SquareShape(g_sideLength);
        auto oct = PolygonShape(8, PolygonShape::SideLength, g_sideLength);
        oct.rotate(360.0f / 16.0f);
        oct.applyTransform();
        auto R = matrix.size();
        auto C = matrix[0].size();
        std::deque<Shape> shapes;
        for (std::size_t r = 0; r < R; ++r)
        {
            for (std::size_t c = 0; c < C; ++c)
            {
                if (matrix[r][c] == 4)
                    shapes.push_back(sqr);
                else if (matrix[r][c] == 8)
                    shapes.push_back(oct);
                oct.move(g_gridSize, 0);
                oct.applyTransform();
                sqr.move(g_gridSize, 0);
                sqr.applyTransform();
            }
            oct.move(-g_gridSize * C, g_gridSize);
            oct.applyTransform();
            sqr.move(-g_gridSize * C, g_gridSize);
            sqr.applyTransform();
        }
        Shape shape;
        while (!shapes.empty())
        {
            auto toMerge = shapes.front();
            shapes.pop_front();
            auto merged = Shape::clipShapes(shape, toMerge, Shape::Union);
            if (merged.size() == 1)
            {
                shape = merged[0];
                *sr->shape = shape;
                co_yield new WaitForSeconds(0.1f);
            }
            else
                shapes.push_back(toMerge);
        }
        *sr->shape = Shape::offsetShape(shape, -2.0f);
    }

    void setCoordinate(const Vector2i& coord) {
        transform.setPosition(coordPosition(coord));
    }

    Matrix matrix;
    Color color;
};

//==============================================================================
// PUZZ
//==============================================================================

class Puzzometry : public GameObject
{
  public:
    Handle<ShapeRenderer> sr;

    Puzzometry() : GameObject("puzzometry")
    {
        board = makeChild<Board>();
        for (std::size_t i = 0; i < g_numPieces; ++i) {
            auto p = makeChild<Piece>(g_matPieces[i], g_colors[i]);
            p->setName("piece_"+str(i));
            p->setCoordinate(g_solutions[i]);
            pieces.push_back(p);
        }
    }

    void update() override {
        if (Input::getKeyDown(Key::Space) && !spinning)
            startCoroutine(spin(0.25f));
    }

    Enumerator spin(float spinTime) {        
        spinning = true;
        float t = 0.0f;
        float rBegin = Engine::getView(0).getRotation() == 315.0f ? 45.0f : 0;
        float rEnd = rBegin == 45.0f ? 0.0f : 45.0f;
        while (t < spinTime) {
            auto r = Tween::Smootheststep(rBegin, rEnd, t/spinTime);
            Engine::getView(0).setRotation(-r);
            t += Engine::deltaTime();
            co_yield nullptr;
        }
        Engine::getView(0).setRotation(-rEnd);
        spinning = false;
    }

    Handle<Board> board;
    std::vector<Handle<Piece>> pieces;
    bool spinning = false;
};

int main(int argc, char const *argv[])
{
    Engine::init();
    Engine::window->setTitle("Puzzometry");
    Engine::window->setFramerateLimit(144);
    // Engine::window->setVerticalSyncEnabled(true);
    Debug::addGizmo("Grid", Grays::Gray50);
    Debug::setGizmoActive(Debug::gizmoId("Grid"), true);
    Debug::setGizmoActive(Debug::gizmoId("Wireframe"), true);
    Debug::setGizmoActive(Debug::gizmoId("Transform"), true);
    Debug::show(true);
    Engine::getView(0).setCenter((size(g_matBoard,0)-1) * g_gridSize / 2.0f, (size(g_matBoard,1)-1) * g_gridSize / 2.0f);
    Engine::makeRoot<Puzzometry>();
    Engine::run();
    return 0;
}

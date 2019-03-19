// #define CARNOT_USE_DISCRETE_GPU
// #define CARNOT_NO_CONSOLE

#include <carnot>
#include <deque>
#include <set>
#include <fstream>

using namespace carnot;

//==============================================================================
// BASIC MATRIX OPERATIONS
//==============================================================================

typedef std::vector<std::vector<int>> Matrix;

std::size_t size(const Matrix& mat, int dim) {
    if (dim == 0)
        return mat.size();
    else if (dim == 1)
        return mat[0].size();
    return 0;
} 

inline Matrix ones(std::size_t r, std::size_t c)
{
    return Matrix(r, std::vector<int>(c, 1));
}

inline Matrix zeros(std::size_t r, std::size_t c)
{
    return Matrix(r, std::vector<int>(c, 0));
}

inline void flipud(Matrix &mat) {
        std::reverse(mat.begin(), mat.end());
}

inline void fliplr(Matrix& mat) {
    for (auto &row : mat)
        std::reverse(row.begin(), row.end());
}

inline void rot90(Matrix& mat) {
    auto temp = mat;
    mat.resize(size(temp,1));
    for (auto j : range(size(temp,1))) {
        mat[j].resize(size(temp,0));
        for (auto i : range(size(temp,0)))
            mat[j][i] = temp[i][j];
    }
    fliplr(mat);
}

void printMat(const Matrix& mat) {
    for (auto i : range(size(mat,0))) {
        for (auto j : range(size(mat,1))) {
            auto val = mat[i][j];
            if (val == 0)
                std::cout << ".  ";
            else
                std::cout << val << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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

const std::size_t g_boardRows = 16;
const std::size_t g_boardCols = 16;

const std::vector<Matrix> g_matPieces
{
    {{8, 4, 8, 4}, {0, 8, 4, 8}},                                          
    {{0, 4, 8}, {4, 8, 4}, {8, 4, 8}},                                     
    {{8, 4, 0}, {4, 8, 4}, {8, 4, 0}},                                     
    {{0, 4, 0}, {4, 8, 4}, {8, 4, 8}, {4, 8, 4}, {8, 4, 0}},               
    {{4, 8, 0, 0}, {8, 4, 8, 0}, {0, 8, 4, 8}, {0, 4, 8, 4}, {0, 8, 4, 0}},
    {{4, 8, 0}, {8, 4, 8}, {0, 8, 4}},                                     
    {{4, 0, 0, 0}, {8, 4, 0, 0}, {4, 8, 4, 0}, {0, 4, 8, 4}},              
    {{0, 0, 0, 8, 4}, {0, 0, 8, 4, 8}, {0, 8, 4, 8, 0}, {8, 4, 0, 0, 0}},  
    {{0, 0, 0, 4, 0}, {0, 8, 4, 8, 4}, {8, 4, 8, 4, 8}, {0, 8, 4, 8, 4}},  
    {{0, 8, 4, 8}, {8, 4, 8, 0}, {4, 8, 4, 0}, {8, 0, 0, 0}},              
    {{0, 4, 8}, {0, 8, 4}, {8, 4, 0}, {0, 8, 0}},                          
    {{8, 4, 8}, {4, 8, 0}, {8, 0, 0}},                                     
    {{4, 8, 0}, {8, 4, 8}, {4, 8, 0}, {8, 0, 0}},                          
    {{4, 8, 0}, {8, 4, 8}, {4, 8, 0}},                                     
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
    Reds::FireBrick,
    Reds::LightCoral,
    Oranges::OrangeRed,
    Yellows::Yellow,
    Greens::YellowGreen,
    Greens::Chartreuse,
    Greens::SpringGreen,
    Cyans::LightSeaGreen,
    Blues::DeepSkyBlue,
    Blues::DodgerBlue,
    Blues::SteelBlue,
    Purples::BlueViolet,
    Purples::Magenta,
    Pinks::DeepPink
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
                // for (int j = 0; j < 16; ++j)
                //     Debug::drawText(str(i)+","+str(j), coordPosition(Vector2i(i,j)), Whites::White);
                Debug::drawLine(coordPosition(-1, i), coordPosition(16, i), Debug::getGizmoColor(id));
                Debug::drawLine(coordPosition(i, -1), coordPosition(i, 16), Debug::getGizmoColor(id));
                Debug::drawText("R"+str(i),coordPosition(Vector2i(i,-2)),Whites::White);
                Debug::drawText("C"+str(i),coordPosition(Vector2i(17,i)),Whites::White);

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

    Handle<ShapeRenderer> sr;
    Handle<StrokeRenderer> lr1, lr2;
    Handle<Trigger> tr;

    Board() : GameObject("board"), matrix(g_matBoard), color(Grays::Gray50)
    {
        addComponent<GridGizmo>();

        sr = addComponent<ShapeRenderer>();
        sr->shape->setPointCount(4);
        sr->shape->setPoint(0, coordPosition(9,-2));
        sr->shape->setPoint(1, coordPosition(-2,9));
        sr->shape->setPoint(2, coordPosition(6,17));
        sr->shape->setPoint(3, coordPosition(17,6));
        tr = addComponent<Trigger>();
        tr->mode = Trigger::Vertices;

        sr->setEffect(make<Gradient>(withAlpha(color,0.25f), withAlpha(color,0.5f), 45.0f));
        sr->shape->setRadii(g_gridSize);
        sr->shape->setHoleCount(1);

        lr1 = addComponent<StrokeRenderer>();
        lr2 = addComponent<StrokeRenderer>();
        lr1->setThickness(1.0f);
        lr2->setThickness(1.0f);

        lr1->setColor(Grays::Gray80);
        lr2->setColor(Grays::Gray80);
        
        lr1->fromShape(*sr->shape);

        startCoroutine(makeShape());
    }

    void onMouseEnter() override {
        lr1->setThickness(5.0f);
        lr2->setThickness(5.0f);
    }

    void onMouseExit() override {
        lr1->setThickness(1.0f);
        lr2->setThickness(1.0f);
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
                sr->shape->setHole(0, shape);
                co_yield nullptr;
            }
            else
                shapes.push_back(toMerge);
        }
        shape = Shape::offsetShape(shape, 2.0f);
        sr->shape->setHole(0, shape);
        lr2->fromShape(shape);
        tr->shape = sr->shape;
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
    Handle<StrokeRenderer> lr;
    Handle<Trigger> tr;

    Piece(const Matrix &mat, const Color &col) : matrix(mat),
                                                 color(col)
    {
        sr = addComponent<ShapeRenderer>();
        tr = addComponent<Trigger>();
        tr->mode = Trigger::Vertices;
        sr->setEffect(make<Gradient>(withAlpha(color,0.25f), withAlpha(color,0.5f) ,45.0f));
        lr = addComponent<StrokeRenderer>();
        lr->setColor(Grays::Gray80);
        startCoroutine(makeShape());
    }

    void onMouseEnter() override {
        lr->setColor(color);
        lr->setThickness(5.0f);
    }

    void onMouseStay() override {
        if (Input::getKeyDown(Key::Left) || Input::getKeyDown(Key::Right))
            flipLR();
        if (Input::getKeyDown(Key::Up) || Input::getKeyDown(Key::Down))
            flipUD();
        if (Input::getKeyDown(Key::Q))
            printMat(matrix);
        if (Input::getKeyDown(Key::P))
            startCoroutine(makeShape());
    }

    void onMouseExit() override  {
        lr->setColor(Grays::Gray80);
        lr->setThickness(1.0f);
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
        tr->shape = sr->shape;
        lr->fromShape(*sr->shape);
    }

    void setCoordinate(const Vector2i& coord) {
        transform.setPosition(coordPosition(coord));
    }

    void flipLR() {
        fliplr(matrix);
        transform.setScale(transform.getScale().x * -1, transform.getScale().y );
    }

    void flipUD() {
        flipud(matrix);
        transform.setScale(transform.getScale().x , transform.getScale().y * -1);
    }

    Matrix matrix;
    Color color;
};

//==============================================================================
// PUZZOMETRY (ROOT OBJECT)
//==============================================================================

class Puzzometry : public GameObject
{
  public:
    Handle<ShapeRenderer> sr;

    Puzzometry() : GameObject("puzzometry")
    {

        sr = addComponent<ShapeRenderer>(make<SquareShape>(2500));
        sr->setEffect(make<Checkerboard>(Grays::Gray10, Grays::Black,250));
        sr->shape->setPosition((size(g_matBoard,0)-1) * g_gridSize / 2.0f, (size(g_matBoard,1)-1) * g_gridSize / 2.0f);
        sr->shape->setScale(-1,1);
        sr->showGizmos = false;

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
            startCoroutine(toggleView(0.25f));
    }

    Enumerator toggleView(float spinTime) {
        toggled = !toggled;
        Debug::show(toggled);        
        spinning = true;
        float t = 0.0f;
        float rBegin = Engine::getView(0).getRotation() == 315.0f ? 45.0f : 0;
        float rEnd = rBegin == 45.0f ? 0.0f : 45.0f;
        while (t < spinTime) {
            auto r = Tween::Smootheststep(rBegin, rEnd, t/spinTime);
            Engine::getView(0).setRotation(-r);
            sr->shape->setRotation(-r);
            t += Engine::deltaTime();
            co_yield nullptr;
        }
        Engine::getView(0).setRotation(-rEnd);
        spinning = false;
    }

    Handle<Board> board;
    std::vector<Handle<Piece>> pieces;
    bool spinning = false;
    bool toggled = true;
};

//==============================================================================
// MAIN
//==============================================================================

struct RowHeader {
    int piece;
    int perm;
    int row;
    int col;
};

Matrix g_exactCoverMatrix;
std::vector<RowHeader> g_exactCoverRowHeaders;

Matrix buildExactCoverMatrix();
void exportExactCoverMatrix(const Matrix& mat);

int main(int argc, char const *argv[])
{   
    g_exactCoverMatrix = buildExactCoverMatrix();
    exportExactCoverMatrix(g_exactCoverMatrix);
    Engine::init(1000,1000,"Puzzometry");
    Debug::addGizmo("Grid", Grays::Gray50);
    Debug::setGizmoActive(Debug::gizmoId("Grid"), true);
    Engine::getView(0).setCenter((size(g_matBoard,0)-1) * g_gridSize / 2.0f, (size(g_matBoard,1)-1) * g_gridSize / 2.0f);
    Debug::show(true);
    Engine::makeRoot<Puzzometry>();
    Engine::run();
    return 0;
}

//============================================================================//
//|||||||||||||||||||||||||||||||| DLX SOLVER ||||||||||||||||||||||||||||||||//
//============================================================================//

void permute(Matrix& mat, int permutation) {
    switch(permutation) {
        case 0 : { break; }
        case 1 : { rot90(mat); break; }
        case 2 : { rot90(mat); rot90(mat); break;}
        case 3 : { rot90(mat); rot90(mat); rot90(mat); break;}
        case 5 : { fliplr(mat); break;}
        case 6 : { fliplr(mat); rot90(mat); break;}
        case 7 : { fliplr(mat); rot90(mat); rot90(mat); break;}
        case 8 : { fliplr(mat); rot90(mat); rot90(mat); rot90(mat); break;}
    }
}

Matrix g_idxMatrix;
std::size_t g_numIdx = 0;

void buildIdxMatrix() {
    g_idxMatrix = zeros(g_boardRows,g_boardCols);
    for (auto r : range(g_boardRows)) {
        for (auto c : range(g_boardCols))
        {
            if (g_matBoard[r][c] != 0)
                g_idxMatrix[r][c] = (int)g_numIdx++;
        }
    }
}

bool place(const Matrix& mat, std::size_t row, std::size_t col, std::vector<int>& ecRowOut) {
    auto height = size(mat,0);
    auto width  = size(mat,1);
    if ((row + height) > g_boardRows || (col + width) > g_boardCols) 
        return false;
    else {
        for (std::size_t r = 0; r < height; ++r) {
            for (std::size_t c = 0; c < width; ++c) {
                if (mat[r][c] == 0)
                    continue;
                auto diff = mat[r][c] - g_matBoard[row + r][col + c];
                if  (diff != 0)
                    return false;
                ecRowOut[g_numPieces + g_idxMatrix[row + r][col + c]] = 1;
            }
        }
    }
    return true;   
}

std::set<Matrix> uniquePerms(const Matrix& mat) {
    std::set<Matrix> perms;
    for (int perm = 0; perm < 8; ++perm) {
        auto temp = mat;
        permute(temp, perm);
        perms.insert(temp);
    }
    return perms;
}

Matrix buildExactCoverMatrix() {
    std::vector<int> ecRow;
    Matrix ecMatrix;
    ecMatrix.reserve(2500);
    buildIdxMatrix();
    for (auto p : range(g_numPieces)) {
        std::set<Matrix> perms = uniquePerms(g_matPieces[p]);
        for (auto& perm : perms) {
            for (auto row : range(g_boardRows)) {
                for (auto col : range(g_boardCols)) {
                    ecRow.assign(g_numPieces + g_numIdx, 0);
                    ecRow[p] = 1;
                    if (place(perm, row, col, ecRow)) {
                        ecMatrix.push_back(ecRow);
                        
                    }
                }
            }
        }
    }
    return ecMatrix;
}

void exportExactCoverMatrix(const Matrix& mat) {
    std::ofstream file;
    file.open("exact_cover_matrix.txt");
    file << size(mat,1) << std::endl;
    for (auto r : range(size(mat,0)))
        file << mat[r] << std::endl;
    file.close();
}
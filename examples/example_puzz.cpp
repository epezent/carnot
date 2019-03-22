// #define CARNOT_USE_DISCRETE_GPU
// #define CARNOT_NO_CONSOLE

#include <carnot>
#include <deque>
#include <set>
#include <fstream>

using namespace carnot;

//==============================================================================
// MATRIX OPERATIONS
//==============================================================================

typedef int              Num;
typedef std::vector<Num> Row;
typedef std::vector<Row> Matrix;

std::size_t size(const Matrix& mat, int dim) {
    if (dim == 0)
        return mat.size();
    else if (dim == 1)
        return mat[0].size();
    return 0;
} 

inline Matrix ones(std::size_t r, std::size_t c) {
    return Matrix(r, Row(c, 1));
}

inline Matrix zeros(std::size_t r, std::size_t c) {
    return Matrix(r, Row(c, 0));
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

void permute(Matrix& mat, std::size_t permutation) {
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

std::size_t uniquePermutations(const Matrix& mat, std::vector<std::size_t>& uniquePerms, std::vector<Matrix>& uniqueMats)  {
    std::size_t numUnique = 0;
    uniquePerms.clear(); uniquePerms.reserve(8);
    uniqueMats.clear();  uniqueMats.reserve(8);
    for (std::size_t perm = 0; perm < 8; ++perm) {
        auto temp = mat;
        permute(temp, perm);
        if (std::find(uniqueMats.begin(), uniqueMats.end(), temp) == uniqueMats.end()) {
            uniquePerms.push_back(perm);  
            uniqueMats.push_back(temp);
            numUnique++;  
        }    
    }
    return numUnique;
}

void sparseToDense(const Matrix& sparse, Matrix& dense) {
    auto numRows = size(sparse,0);
    auto numCols = size(sparse,1);
    dense.assign(numRows,{});
    for (std::size_t r = 0; r < numRows; ++r) {
        for (std::size_t c = 0; c < numCols; ++c) {
            if (sparse[r][c])
                dense[r].push_back((Num)c);
        }
    }
}

std::size_t nnz(const Matrix& mat) {
    std::size_t n = 0;
    for (auto r : range(size(mat, 0))) {
        for (auto c : range(size(mat, 1))) {
            if (mat[r][c] != 0)
                n++;
        }
    }
    return n;
}

void printMat(const Matrix& mat) {
    for (auto& row : mat) {
        for (auto& val : row) {
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

const Matrix g_board {
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

const std::vector<Matrix> g_pieces
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

Shape makeShape(const Matrix& mat) {
        // create square and octagon primitives
        auto sqr = SquareShape(g_sideLength);
        auto oct = PolygonShape(8, PolygonShape::SideLength, g_sideLength);
        oct.rotate(360.0f / 16.0f); oct.applyTransform();
        // create que of shapes to merge
        std::deque<Shape> shapes;
        for (auto & r : range(size(mat,0))) {
            for (auto & c : range(size(mat,1))) {
                if (mat[r][c] == 4)
                    shapes.push_back(sqr);
                else if (mat[r][c] == 8)
                    shapes.push_back(oct);
                oct.move(g_gridSize, 0);  oct.applyTransform();
                sqr.move(g_gridSize, 0);  sqr.applyTransform();
            }
            oct.move(-g_gridSize * size(mat,1), g_gridSize);
            oct.applyTransform();
            sqr.move(-g_gridSize * size(mat,1), g_gridSize);
            sqr.applyTransform();
        }
        // marge shapes
        Shape shape;
        while (!shapes.empty()) {
            auto toMerge = shapes.front();
            shapes.pop_front();
            auto merged = Shape::clipShapes(shape, toMerge, Shape::Union);
            if (merged.size() == 1)
                shape = merged[0];
            else
                shapes.push_back(toMerge);
        }
        return shape;
};

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

    Board() : GameObject("board"), matrix(g_board), color(Grays::Gray50) {
        addComponent<GridGizmo>();
        // board shape
        sr = addComponent<ShapeRenderer>();
        sr->setEffect(make<Gradient>(withAlpha(color, 0.25f), withAlpha(color,0.5f), 45.0f));
        sr->getShape()->setPointCount(4);
        sr->getShape()->setPoint(0, coordPosition(9,-2));
        sr->getShape()->setPoint(1, coordPosition(-2,9));
        sr->getShape()->setPoint(2, coordPosition(6,17));
        sr->getShape()->setPoint(3, coordPosition(17,6));
        sr->getShape()->setRadii(g_gridSize);
        // board shape hole
        auto hole = makeShape(matrix);
        hole = Shape::offsetShape(hole, 2.0f);
        sr->getShape()->addHole(hole);
        // outlines
        lr1 = addComponent<StrokeRenderer>();
        lr2 = addComponent<StrokeRenderer>();
        lr1->setThickness(1.0f);
        lr2->setThickness(1.0f);
        lr1->setColor(Grays::Gray80);
        lr2->setColor(Grays::Gray80);        
        lr1->fromShape(*sr->getShape());
        lr2->fromShape(sr->getShape()->getHole(0));
        // trigger
        tr = addComponent<Trigger>();
        tr->mode = Trigger::Vertices;
        tr->shape = sr->getShape();
    }

    void onMouseEnter() override {
        lr1->setThickness(5.0f);
        lr2->setThickness(5.0f);
    }

    void onMouseExit() override {
        lr1->setThickness(1.0f);
        lr2->setThickness(1.0f);
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
        // piece shape
        sr = addComponent<ShapeRenderer>();
        sr->setEffect(make<Gradient>(withAlpha(color,0.25f), withAlpha(color,0.5f) ,45.0f));
        auto shape = makeShape(matrix);
        sr->setShape(Shape::offsetShape(shape, -2.0f));
        // outline
        lr = addComponent<StrokeRenderer>();
        lr->setColor(Grays::Gray80);
        lr->fromShape(*sr->getShape());
        // trigger
        tr = addComponent<Trigger>();
        tr->mode = Trigger::Vertices;
        tr->shape = sr->getShape();
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
    }

    void onMouseExit() override  {
        lr->setColor(Grays::Gray80);
        lr->setThickness(1.0f);
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
        auto sqr = make<SquareShape>(2500);
        sr = addComponent<ShapeRenderer>(sqr);
        sr->setEffect(make<Checkerboard>(Grays::Gray10, Grays::Black, 250));
        sr->getShape()->setPosition((size(g_board,0)-1) * g_gridSize / 2.0f, (size(g_board,1)-1) * g_gridSize / 2.0f);
        sr->getShape()->setScale(-1,1);
        sr->showGizmos = false;

        board = makeChild<Board>();
        for (std::size_t i = 0; i < g_pieces.size(); ++i) {
            auto p = makeChild<Piece>(g_pieces[i], g_colors[i]);
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
            sr->getShape()->setRotation(-r);
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
// DLX EXACT COVER SOLVER
//==============================================================================



/// Formulates an exact cover problem from a board and number of pieces
struct ExactCoverProblem {

    struct Info {
        std::size_t piece, perm, r, c;
    };

    ExactCoverProblem(const Matrix& board, const std::vector<Matrix>& pieces) :
        board(board), pieces(pieces), numPieces(pieces.size()), numSlots(0),
        boardRows(size(board,0)), boardCols(size(board,1))
    {
        buildSlots();
        buildSparseMatrix();
        sparseToDense(sparse,dense);
    }

    void buildSlots() {
        slots = zeros(size(board, 0), size(board, 1));
        for (auto r : range(size(slots,0))) {
            for (auto c : range(size(slots,1))) {
                if (board[r][c] != 0)
                    slots[r][c] = (Num)numSlots++;
            }
        }
    }

    void buildSparseMatrix() {
        Row row;
        sparse.reserve(2500);
        dense.reserve(2500);
        info.reserve(2500);
        for (auto piece : range(numPieces)) {
            std::vector<std::size_t> uniquePerms;
            std::vector<Matrix>      uniqueMats;
            auto numUnique = uniquePermutations(pieces[piece], uniquePerms, uniqueMats);
            for (auto perm : range(numUnique)) {
                for (auto r : range(boardRows)) {
                    for (auto c : range(boardCols)) {
                        row.assign(numPieces + numSlots, 0);
                        row[piece] = 1;
                        if (place(uniqueMats[perm], r, c, row)) {
                            sparse.push_back(row);  
                            info.push_back({piece, perm, r, c});
                        }                     
                    }
                }
            }
        }
        numRows = size(sparse,0);
        numCols = size(sparse,1);
    }

   
    bool place(const Matrix& permMat, std::size_t r, std::size_t c, Row& rowOut) {
        auto height = size(permMat,0);
        auto width  = size(permMat,1);
        if ((r + height) > boardRows || (c + width) > boardCols) 
            return false;
        else {
            for (std::size_t rr = 0; rr < height; ++rr) {
                for (std::size_t cc = 0; cc < width; ++cc) {
                    if (permMat[rr][cc] == 0)
                        continue;
                    auto diff = permMat[rr][cc] - board[r + rr][c + cc];
                    if  (diff != 0)
                        return false;
                    rowOut[numPieces + slots[r + rr][c + cc]] = 1;
                }
            }
        }
        return true;   
    }

    void writeToFiles() {
        std::ofstream file1, file2, file3;
        file1.open("exact_cover_sparse.txt");
        file2.open("exact_cover_dense.txt");
        file3.open("exact_cover_info.txt");
        for (auto r : range(numRows)) {
            file1 << sparse[r] << std::endl;
            file2 << dense[r]  << std::endl;
            file3 << str(info[r].piece, info[r].perm, info[r].r, info[r].c) << std::endl;
        }
        file1.close();
        file2.close();
        file3.close();
    }    

    Matrix              board;
    std::vector<Matrix> pieces;
    std::size_t         boardRows;
    std::size_t         boardCols;
    std::size_t         numPieces;
    std::size_t         numSlots;
    Matrix              slots;
    Matrix              sparse;
    Matrix              dense;
    std::size_t         numRows;
    std::size_t         numCols;
    std::size_t         numCells;
    std::vector<Info>   info;
};

struct DLX {

    struct Cell {
        Cell(int r, int c) : r(r), c(c) {}
        int r, c;
        Cell* R = nullptr; // pointer to right cell
        Cell* L = nullptr; // pointer to left cell
        Cell* U = nullptr; // pointer to up cell
        Cell* D = nullptr; // pointer to down cell
        Cell* C = nullptr; // pointer to header cell
    };  

    DLX(const Matrix& dense, std::size_t numCols) {
        std::size_t numCells = 1 + numCols;
        for (auto& row : dense)
            numCells += row.size();
        cells.reserve(numCells);
        sizes.resize(numCols);
        headers.resize(numCols);
        makeHeaderRow(numCols);
        for (std::size_t r = 0; r < dense.size(); ++r)
            makeRow(r, dense[r]);
        assert(numCells == cells.size());
    }

    void makeHeaderRow(std::size_t numCols) {
        // root
        root = &cells.emplace_back(-1,-1);
        root->L = root;
        root->R = root;
        // headers
        for (std::size_t c = 0; c < numCols; ++c) {
            auto h = &cells.emplace_back(-1, c);
            // up/down linkage
            h->U = h;
            h->D = h;
            // left/right linkage
            h->L = root->L;
            h->L->R = h;
            root->L = h;
            h->R = root;
            // linkage to header (self)
            h->C = h;
            // insert into header list
            headers[c] = h;
        }
    }

    void makeRow(std::size_t r, const Row& row) {
        Cell* leftMost = nullptr;
        for (auto& c : row) {
            auto cell = &cells.emplace_back(r, c);
            // up/down linkage
            cell->U = headers[c]->U;
            cell->U->D = cell;
            headers[c]->U = cell;
            cell->D = headers[c];
            // left/right linkage
            if (!leftMost) {
                cell->L = cell;
                cell->R = cell;    
                leftMost = cell;        
            }
            else {
                cell->L     = leftMost->L;
                cell->L->R  = cell;
                leftMost->L = cell;
                cell->R     = leftMost;
            }
            // linkage to header
            cell->C = headers[c];
            ++sizes[c];
        }
    }

    std::size_t chooseColumn() {
        std::size_t c = root->R->c;
        std::size_t s = -1;        
        for (Cell* j = root->R; j != root; j = j->R) {
            if (sizes[j->c] < s) {
                s = sizes[j->c];
                c = j->c;
            }
        }
        return c;
    }

    void cover(std::size_t c) {
        headers[c]->R->L = headers[c]->L;
        headers[c]->L->R = headers[c]->R;
        for (Cell* i = headers[c]->D; i != headers[c]; i = i->D) {            
            for (Cell* j = i->R; j != i; j = j->R) {
                j->D->U = j->U;
                j->U->D = j->D;
                sizes[j->c]--;
            }
        }
    }

    void uncover(std::size_t c) {        
        for (Cell* i = headers[c]->U; i != headers[c]; i = i->U) {            
            for (Cell* j = i->L; j != i; j = j->L) {
                sizes[j->c]++;
                j->D->U = j;
                j->U->D = j;
            }
        }
        headers[c]->R->L = headers[c];
        headers[c]->L->R = headers[c];
    }

    void search(int k = 0) {
        iterations = k == 0 ? 0 : iterations + 1;
        if (root->R == root) {
            solutions.push_back(workingSolution);
            return;
        }
        auto c = chooseColumn();
        cover(c);        
        for (Cell* r = headers[c]->D; r != headers[c]; r = r->D) {
            workingSolution.push_back(r->r);           
            for (Cell* j = r->R; j != r; j = j->R) 
                cover(j->c);                
            search(k + 1);
            workingSolution.pop_back();
            headers[c] = headers[r->c];            
            for (Cell* j = r->L; j != r; j = j->L)
                uncover(j->c);                
        }
        uncover(c);
    }    

    Cell* root;
    std::vector<Cell*> headers;
    std::vector<Cell> cells;
    std::vector<int> sizes;
    std::vector<std::size_t> workingSolution;
    std::vector<std::vector<std::size_t>> solutions;
    std::size_t iterations = 0;
};


//==============================================================================
// MAIN
//==============================================================================

int main(int argc, char const *argv[])
{   
    Clock clock;
    ExactCoverProblem problem(g_board, g_pieces);
    DLX dlx(problem.dense, problem.numCols);
    dlx.search(0);
    print("Time:", clock.getElapsedTime().asMicroseconds(), "us");
    print("Iterations:", dlx.iterations);
    print("Solutions:", dlx.solutions.size());

    std::ofstream file;
    file.open("solution.txt");
    for (auto& idx : dlx.solutions[0]) {
        auto row = problem.sparse[idx];
        file << row << std::endl;
    }
    file.close();



    // Engine::init(1000,1000,"Puzzometry");
    // Debug::addGizmo("Grid", Grays::Gray50);
    // Debug::setGizmoActive(Debug::gizmoId("Grid"), true);
    // Engine::getView(0).setCenter((size(g_board,0)-1) * g_gridSize / 2.0f, (size(g_board,1)-1) * g_gridSize / 2.0f);
    // Debug::show(true);
    // Engine::makeRoot<Puzzometry>();
    // Engine::run();
    return 0;
}
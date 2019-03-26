// #define CARNOT_USE_DISCRETE_GPU
// #define CARNOT_NO_CONSOLE

#include <carnot>
#include <deque>
#include <set>
#include <fstream>
#include <utility>

using namespace carnot;
using std::size_t;
using std::vector;
using std::pair;

//==============================================================================
// MATRIX OPERATIONS
//==============================================================================

typedef int         Num;
typedef vector<Num> Row;
typedef vector<Row> Matrix;

inline size_t size(const Matrix& mat, int dim) {
    if (dim == 0)
        return mat.size();
    else if (dim == 1)
        return mat[0].size();
    return 0;
} 

inline Matrix zeros(size_t r, size_t c) {
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

inline void permute(Matrix& mat, size_t permutation) {
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

inline size_t uniquePermutations(const Matrix& mat, vector<size_t>& uniquePerms, vector<Matrix>& uniqueMats)  {
    size_t numUnique = 0;
    uniquePerms.clear(); uniquePerms.reserve(8);
    uniqueMats.clear();  uniqueMats.reserve(8);
    for (size_t perm = 0; perm < 8; ++perm) {
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

inline void sparseToDense(const Matrix& sparse, Matrix& dense) {
    auto numRows = size(sparse,0);
    auto numCols = size(sparse,1);
    dense.assign(numRows,{});
    for (size_t r = 0; r < numRows; ++r) {
        for (size_t c = 0; c < numCols; ++c) {
            if (sparse[r][c])
                dense[r].push_back((Num)c);
        }
    }
}

std::string matToStr(const Matrix& mat) {
    std::stringstream ss;
    for (auto& row : mat) {
        for (auto& val : row) {
            if (val == 0)
                ss << ". ";
            else
                ss << val << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

inline void printMat(const Matrix& mat) {
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

const vector<Matrix> g_pieces
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

const vector<Vector2i> g_solutions {
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

const vector<Color> g_colors {
    Reds::FireBrick,
    Oranges::OrangeRed,
    Reds::LightCoral,
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
// DLX EXACT COVER SOLVER
//==============================================================================

/// Formulates an exact cover problem from a board and number of pieces
struct ExactCover {

    struct Info {
        size_t piece, perm, r, c;
    };

    ExactCover(const Matrix& board, const vector<Matrix>& pieces) :
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
            vector<size_t> uniquePerms;
            vector<Matrix>      uniqueMats;
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

   
    bool place(const Matrix& permMat, size_t r, size_t c, Row& rowOut) {
        auto height = size(permMat,0);
        auto width  = size(permMat,1);
        if ((r + height) > boardRows || (c + width) > boardCols) 
            return false;
        else {
            for (size_t rr = 0; rr < height; ++rr) {
                for (size_t cc = 0; cc < width; ++cc) {
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

    Matrix         board;
    vector<Matrix> pieces;
    size_t         boardRows;
    size_t         boardCols;
    size_t         numPieces;
    size_t         numSlots;
    Matrix         slots;
    Matrix         sparse;
    Matrix         dense;
    size_t         numRows;
    size_t         numCols;
    vector<Info>   info;
};

/// Implementation of Donald Knuth's DLX algorithm
struct DLX {
    /// Vector of row indices into exact cover matrix
    typedef vector<size_t> Solution;
    /// Node within toroidal linked list
    struct Cell {
        Cell(int row) : row(row) {}
        int row;              // row of cell
        Cell* R    = nullptr; // pointer to right cell
        Cell* L    = nullptr; // pointer to left cell
        Cell* U    = nullptr; // pointer to up cell
        Cell* D    = nullptr; // pointer to down cell
        Cell* C    = nullptr; // pointer to header cell
        size_t* S  = nullptr; // pointer to cell's column size
    };  
    /// Constructor, taking exact cover matrix in dense representation
    DLX(const Matrix& dense, size_t numCols) {
        size_t numCells = 1 + numCols;
        for (auto& row : dense)
            numCells += row.size();
        cells.reserve(numCells);
        sizes.assign(numCols,0);
        headers.resize(numCols);
        makeHeaderRow(numCols);
        for (size_t r = 0; r < dense.size(); ++r)
            makeRow(r, dense[r]);
        assert(numCells == cells.size());
    }
    /// Makes root and header cells
    void makeHeaderRow(size_t numCols) {
        // root
        root = &cells.emplace_back(-1);
        root->L = root;
        root->R = root;
        // headers
        for (size_t c = 0; c < numCols; ++c) {
            auto h = &cells.emplace_back(-1);
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
            /// linage to size block
            h->S = &sizes[c];
            // insert into header list
            headers[c] = h;
        }
    }
    /// Takes a dense row representation and adds it to toroidal linked list
    void makeRow(size_t r, const Row& row) {
        Cell* leftMost = nullptr;
        for (auto& c : row) {
            auto cell = &cells.emplace_back(r);
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
            // linkage to size block
            cell->S = &sizes[c];
            ++sizes[c];
        }
    }
    /// Chooses column with fewest cells remaining in it
    inline Cell* chooseColumn() {
        auto C = root->R->C;
        size_t s = -1;        
        for (Cell* j = root->R; j != root; j = j->R) {
            if (*j->S < s) {
                s = *j->S;
                C = j->C;
            }
        }
        return C;
    }
    /// Covers a column cell C
    inline void cover(Cell* C) {
        C->R->L = C->L;
        C->L->R = C->R;
        for (Cell* i = C->D; i != C; i = i->D) {            
            for (Cell* j = i->R; j != i; j = j->R) {
                j->D->U = j->U;
                j->U->D = j->D;
                --*j->S;
            }
        }
    }
    /// Uncoverse a column cell C
    inline void uncover(Cell* C) {        
        for (Cell* i = C->U; i != C; i = i->U) {            
            for (Cell* j = i->L; j != i; j = j->L) {
                ++*j->S;
                j->D->U = j;
                j->U->D = j;
            }
        }
        C->R->L = C;
        C->L->R = C;
    }
    /// Recursive search algorithm
    bool search() {
        if (root->R == root) {
            solutions.push_back(workingSolution);
            return true;
        }
        auto C = chooseColumn();
        cover(C);        
        for (Cell* r = C->D; r != C; r = r->D) {
            workingSolution.push_back(r->row);  
            operations.push_back({(size_t)r->row,true});         
            for (Cell* j = r->R; j != r; j = j->R) 
                cover(j->C);                
            if (search() && !findAll)
                return true;
            workingSolution.pop_back();
            operations.push_back({(size_t)r->row,false});
            C = r->C;           
            for (Cell* j = r->L; j != r; j = j->L)
                uncover(j->C);                
        }
        uncover(C);
        return false;
    }    

    bool                      findAll = false; ///< find all solutions?
    vector<Cell>              cells;           ///< all root, header, and normal cells
    Cell*                     root;            ///< pointer to root cell
    vector<Cell*>             headers;         ///< pointers to headers indexed by column
    vector<size_t>            sizes;           ///< column sizes indexed by columns
    Solution                  workingSolution; ///< the working solution of search()
    vector<Solution>          solutions;       ///< all final saved solutions of search()
    vector<pair<size_t,bool>> operations;      ///< row insertions (true) and deletions (false)
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
        oct.rotate(360.0f / 16.0f);
        // create que of shapes to merge
        std::deque<Shape> shapes;
        for (auto & r : range(size(mat,0))) {
            for (auto & c : range(size(mat,1))) {
                if (mat[r][c] == 4)
                    shapes.push_back(sqr);
                else if (mat[r][c] == 8)
                    shapes.push_back(oct);
                oct.move(g_gridSize, 0);  
                sqr.move(g_gridSize, 0);
            }
            oct.move(-g_gridSize * size(mat,1), g_gridSize);
            sqr.move(-g_gridSize * size(mat,1), g_gridSize);
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
                Debug::drawLine(gameObject.transform.localToWorld(coordPosition(-1, i)), gameObject.transform.localToWorld(coordPosition(16, i)), Debug::getGizmoColor(id));
                Debug::drawLine(gameObject.transform.localToWorld(coordPosition(i, -1)), gameObject.transform.localToWorld(coordPosition(i, 16)), Debug::getGizmoColor(id));
                Debug::drawText("R"+str(i),gameObject.transform.localToWorld(coordPosition(Vector2i(i,-2))),Whites::White);
                Debug::drawText("C"+str(i),gameObject.transform.localToWorld(coordPosition(Vector2i(17,i))),Whites::White);

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

    Board() : GameObject("Board"), matrix(g_board), color(Grays::Gray50) {
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
        lr1->setThickness(3.0f);
        lr2->setThickness(3.0f);
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

    Piece(const Matrix &mat, const Color &col) : m_matrix(mat),
                                                 m_color(col)
    {
        // piece shape
        sr = addComponent<ShapeRenderer>();
        sr->setEffect(make<Gradient>(withAlpha(m_color,0.25f), withAlpha(m_color,0.5f) ,45.0f));
        auto shape = makeShape(m_matrix);
        sr->setShape(Shape::offsetShape(shape, -2.0f));
        // outline
        lr = addComponent<StrokeRenderer>();
        lr->setColor(Tween::Linear(Whites::White, m_color, 0.5f));
        lr->fromShape(*sr->getShape());
        // trigger
        tr = addComponent<Trigger>();
        tr->mode = Trigger::Vertices;
        tr->shape = sr->getShape();
        // perm
        m_perm = 0;
    }

    void onMouseEnter() override {
        lr->setColor(m_color);
        lr->setThickness(3.0f);
    }

    void onMouseStay() override {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_color, getName().c_str());
        ImGui::Text("R:%i C:%i", m_coord.x, m_coord.y);
        ImGui::Separator();
        auto permMat = m_matrix;
        permute(permMat, m_perm);
        auto matStr = matToStr(permMat);
        ImGui::Text(matStr.c_str());
        ImGui::EndTooltip();
        if (Input::getMouseDown(MouseButton::Left))
            m_selected = !m_selected;
    }

    void onMouseExit() override  {
        lr->setColor(Tween::Linear(Whites::White, m_color, 0.5f));
        lr->setThickness(1.0f);
    }

    void setCoordinate(const Vector2i& coord) {
        m_coord = coord;
        transform.setLocalPosition(coordPosition(coord));
    }

    void update() override {
        if (m_selected) {
            lr->setColor(m_color);
            lr->setThickness(5.0f);
            if (Input::getKeyDown(Key::Left))
                setCoordinate(m_coord + Vector2i(0,-1));
            if (Input::getKeyDown(Key::Right))
                setCoordinate(m_coord + Vector2i(0,1));
            if (Input::getKeyDown(Key::Up))
                setCoordinate(m_coord + Vector2i(-1,0));
            if (Input::getKeyDown(Key::Down))
                setCoordinate(m_coord + Vector2i(1,0)); 
        }
    }

    Enumerator transition(Vector2i coord, std::size_t perm, float duration) {
        transitioning = true;
        sr->setLayer(1);
        lr->setLayer(1);
        // start state
        auto startPosition = transform.getLocalPosition();
        auto startRotation = transform.getLocalRotation();
        auto startScale    = transform.getLocalScale();
        auto startOrigin   = transform.getLocalOrigin();
        // end state
        auto endPosition   = coordPosition(coord);
        auto endRotation   = 90.0f * (float)(perm % 4);
        auto endScale      = perm < 4 ? Vector2f(1,1) : Vector2f(-1,1);
        auto endOrigin     = g_gridSize * Vector2f((perm == 2 || perm == 3 || perm == 4 || perm == 5) ? (float)size(m_matrix,1) - 1 : 0.0f, 
                                                   (perm == 1 || perm == 2 || perm == 5 || perm == 6) ? (float)size(m_matrix,0) - 1 : 0.0f);
        // animation loop
        float elapsedTime = 0.0f;
        while (elapsedTime < duration) {
            float t  = elapsedTime / duration;
            transform.setLocalPosition(Tween::Smootherstep(startPosition, endPosition, t));
            transform.setLocalRotation(Tween::Smootherstep(startRotation, endRotation, t));
            transform.setLocalScale(Tween::Smootherstep(startScale, endScale, t));
            transform.setLocalOrigin(Tween::Smootherstep(startOrigin, endOrigin, t));
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }        
        transform.setLocalPosition(endPosition);
        transform.setLocalRotation(endRotation);
        transform.setLocalScale(endScale);
        transform.setLocalOrigin(endOrigin);
        m_perm = perm;
        sr->setLayer(0);
        sr->setLayer(1);
        transitioning = false;        
    }

    bool transitioning = false;
    bool m_selected = false;
    std::size_t m_perm;
    Vector2i   m_coord;
    Matrix    m_matrix;
    Color      m_color;
};

//==============================================================================
// PUZZOMETRY (ROOT OBJECT)
//==============================================================================

class Puzzometry : public GameObject
{
  public:
    Handle<ShapeRenderer> sr;

    Puzzometry() : GameObject("Puzzometry"), problem(g_board, g_pieces), solver(problem.dense, problem.numCols)
    {
        auto sqr = make<SquareShape>(2500);
        sr = addComponent<ShapeRenderer>(sqr);
        sr->setEffect(make<Checkerboard>(Grays::Gray10, Grays::Black, 250));
        sr->showGizmos = false;

        board = makeChild<Board>();
        board->transform.setLocalOrigin((size(g_board,0)-1) * g_gridSize / 2.0f, (size(g_board,1)-1) * g_gridSize / 2.0f);
        for (size_t i = 0; i < g_pieces.size(); ++i) {
            auto p = board->makeChild<Piece>(g_pieces[i], g_colors[i]);
            p->setName(str("Piece",i));
            p->setCoordinate(g_solutions[i]);
            pieces.push_back(p);
        }

        // solve
        solver.search();
    }

    Enumerator animateSolution() {
        animating = true;
        for (auto p : pieces)
            co_yield startCoroutine(p->transition(Vector2i(-4,-4), 0, 0.1f));
        co_yield new WaitForSeconds(0.5f);
        for (auto i : range(solver.operations.size())) {
            auto op = solver.operations[i];
            auto info = problem.info[op.first];
            auto piece = pieces[info.piece];
            if (op.second) {
                // insetion
                co_yield startCoroutine(piece->transition(Vector2i(info.r,info.c), info.perm, 0.25f));
                co_yield new WaitForSeconds(0.15f);
            }
            else {
                // removal
                auto nextOp = solver.operations[(i+1)%solver.operations.size()];
                auto nextInfo = problem.info[nextOp.first];
                if (info.piece != nextInfo.piece)
                    co_yield startCoroutine(piece->transition(Vector2i(-4,-4), piece->m_perm, 0.1f));
            }
        }
        animating = false;
    }

    void update() override {
        if (Input::getKeyDown(Key::Space) && !spinning)
            startCoroutine(toggleView(0.25f));
        if (Input::getKeyDown(Key::S) && !animating)
            startCoroutine(animateSolution());
    }

    Enumerator toggleView(float spinTime) {
        toggled = !toggled;
        Debug::show(toggled);        
        spinning = true;
        float t = 0.0f;
        float rBegin = board->transform.getRotation();
        float rEnd = 45.0f - rBegin;
        while (t < spinTime) {
            auto r = Tween::Smootheststep(rBegin, rEnd, t/spinTime);
            board->transform.setRotation(r);
            t += Engine::deltaTime();
            co_yield nullptr;
        }
        board->transform.setRotation(rEnd);
        spinning = false;
    }

    ExactCover problem;
    DLX solver;
    Handle<Board> board;
    vector<Handle<Piece>> pieces;
    bool spinning = false;
    bool animating = false;
    bool toggled = true;
};

//==============================================================================
// MAIN
//==============================================================================

int main(int argc, char const *argv[])
{   
    Engine::init("Puzzometry");
    Engine::setLayerCount(2);
    Debug::addGizmo("Grid", Grays::Gray50);
    Debug::setGizmoActive(Debug::gizmoId("Grid"), true);
    Engine::getView(0).setCenter(0,0);
    Debug::show(true);
    Engine::makeRoot<Puzzometry>();
    Engine::run();
    return 0;
}
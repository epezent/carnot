#include <SFVG/SFVG.hpp>

using namespace sfvg;

const float SIDE_LENGTH = 30.0f;
const float GRID_SIZE   = (2.0f + sqrt(2.0f)) * 0.5f * SIDE_LENGTH * 0.999f;

//==============================================================================
// BASIC MATRIX OPERATIONS
//==============================================================================

typedef std::vector<std::vector<char>> Matrix;

inline Matrix ones(std::size_t r, std::size_t c) {
    return Matrix(r,std::vector<char>(c, 1));
}

inline Matrix zeros(std::size_t r, std::size_t c) {
    return Matrix(r,std::vector<char>(c, 0));
}

inline void flipLR(Matrix& mat) {
    for (auto& row : mat)
        std::reverse(row.begin(),row.end());
}

inline void flipUD(Matrix& mat) {
    std::reverse(mat.begin(), mat.end());
}

bool allZeros(const Matrix& mat) {
    for (auto& row : mat)
        for (auto& col : row)
            if (col != 0)
                return false;
    return true;
}

Matrix board = {{0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 0, 0, 0, 0, 0, 0},
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
                {0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

//==============================================================================
// PIECE
//==============================================================================

class Piece : public GameObject {
public:

    Handle<ShapeRenderer> sr;

    Piece(const Matrix& mat, const Color& col) :
        matrix(mat),
        color(col)
    {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(color);
        // makeShape();
    }

    Enumerator makeShape() {
        auto sqr = SquareShape(SIDE_LENGTH);
        auto oct = PolygonShape(8, PolygonShape::SideLength, SIDE_LENGTH);
        oct.rotate(360.0f/16.0f);
        oct.applyTransform();
        Shape shape;
        auto R = matrix.size();
        auto C = matrix[0].size();
        auto check = ones(R,C);        
            for (std::size_t r = 0; r < R; ++r) {
                for (std::size_t c = 0; c < C; ++c) {
                    if (matrix[r][c] == 4) {
                        auto shapes = Shape::clipShapes(shape, sqr, Shape::Union);
                        if (shapes.size() > 0) {
                            check[r][c] = 0;
                            shape = shapes[0];
                            // shape.setHoleCount(0);
                        }
                    }
                    else if (matrix[r][c] == 8) {
                        auto shapes =Shape::clipShapes(shape, oct, Shape::Union);
                        if (shapes.size() > 0) {
                            check[r][c] = 0;
                            shape = shapes[0];
                            // shape.setHoleCount(0);
                        }
                    }  
                    oct.move(GRID_SIZE, 0); oct.applyTransform();
                    sqr.move(GRID_SIZE, 0); sqr.applyTransform();
                    *sr->shape = shape;  
                    if (matrix[r][c] != 0)     
                        co_yield nullptr;
                }
                oct.move(-GRID_SIZE * C, GRID_SIZE); oct.applyTransform();
                sqr.move(-GRID_SIZE * C, GRID_SIZE); sqr.applyTransform();
            }
        // shape = Shape::offsetShape(shape,-2.0f);
        print("Done!");
    }

    void update() {
        auto localPos = transform.worldToLocal(Input::getMousePosition());
        if (sr->shape->isInside(localPos)) {
            sr->setColor(Whites::White);
            transform.move(Input::dragDelta(MouseButton::Left));
        }
        else {
            sr->setColor(color);
        }

        auto scale = transform.getScale();
        if (Input::getKeyDown(Key::Left) || Input::getKeyDown(Key::Right))
            transform.setScale(-1 * scale.x ,scale.y);
        else if (Input::getKeyDown(Key::Up) || Input::getKeyDown(Key::Down))
            transform.setScale(scale.x , -1 * scale.y);

        if (Input::getKeyDown(Key::Space))
            startCoroutine(makeShape());

    }        

    Matrix matrix;
    Color  color;
};

//==============================================================================
// PUZZ
//==============================================================================

class Puzz : public GameObject {
public:

    Handle<ShapeRenderer> sr;

    Puzz() {
        flipUD(board);
        flipLR(board);
        makeChild<Piece>(board, Grays::Gray50)->transform.setPosition(-350,-350);
        // makeChild<Piece>(Matrix{{8, 4, 8, 4},{0, 8, 4, 8}}, Reds::FireBrick);
        // makeChild<Piece>(Matrix{{8, 4, 0}, {4, 8, 4}, {8, 4, 8}}, Oranges::Orange);
        // makeChild<Piece>(Matrix{{8, 4, 0}, {4, 8, 4}, {8, 4, 0}}, Yellows::Gold);
        // makeChild<Piece>(Matrix{{8, 4, 0}, {4, 8, 4}, {8, 4, 8}, {4, 8, 4}, {0, 4, 0}}, Yellows::Yellow);
        // makeChild<Piece>(Matrix{{4, 8, 0, 0},{8, 4, 8, 0},{0, 8, 4, 8},{0, 4, 8, 4},{0, 8, 4, 0}}, Greens::Chartreuse);        
        // makeChild<Piece>(Matrix{{4, 8, 0}, {8, 4, 8}, {0, 8, 4}}, Greens::Lime);
        //makeChild<Piece>(Matrix{{4, 0, 0, 0}, {8, 4, 0, 0}, {4, 8, 4, 0}, {0, 4, 8, 4}}, Greens::Green);

    }
};

int main(int argc, char const *argv[])
{
    Engine::init(1000,1000);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<Puzz>();
    Debug::show(true);
    Engine::run();
    return 0;
}

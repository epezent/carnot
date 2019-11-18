#include <carnot>

using namespace carnot;

static std::vector<Color> darks  = {hexCode("09489A"),hexCode("13815D"),hexCode("DA6100"),hexCode("DA1F00"),hexCode("660013"),hexCode("34006F")};
static std::vector<Color> lights = {hexCode("0BF0E7"),hexCode("ACFF1C"),hexCode("FFEA03"),hexCode("FF9000"),hexCode("FD1E32"),hexCode("BF01FD")};

// Hexagon Demenisions
float hexS = 50;  // pixels
float hexW = 100; // pixels
float hexH = 86.6f;  // pixels   

int rCenter = 6; 
int cCenter = 5;
int rTotal = 13;
int cTotal = 11;    

// Board Identity
std::vector<std::vector<int>> boardIdent =
{               
    {1,1,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,1,1},        
};


class Piece : public GameObject {
public:

    enum Type {
        Blue, Green, Yellow, Orange, Red, Purple
    };

    Piece::Piece(Type type, int r, int c) {
        sr1 = addComponent<ShapeRenderer>();
        sr2 = addComponent<ShapeRenderer>();        

        auto outer = make<PolygonShape>();
        outer->setCircumscribedRadius(46);
        outer->rotate(30);
        outer->setRadii(11);
        sr1->setEffect(make<Gradient>(darks[type], lights[type],240));
        sr1->setShape(outer);

        auto inner = make<PolygonShape>();
        inner->setCircumscribedRadius(37);
        inner->rotate(30);
        inner->setRadii(11);
        sr2->setEffect(make<Gradient>(darks[type], lights[type],60));
        sr2->setShape(inner);

        // tr = addComponent<TextRenderer>();
        // tr->text.setString(str(r,"x",c));
        // tr->text.setFillColor(Color::Black);
        // tr->text.setCharacterSize(15);
        // alignCenter(tr->text);

    }

private:
    Handle<ShapeRenderer> sr1;
    Handle<ShapeRenderer> sr2;
    Handle<TextRenderer>  tr;
};

class Board : public GameObject {
public:
    Board() {
        for (int c = 0; c < cTotal; c++)
        {
            for (int r = 0; r < rTotal; r++)
            {                
                if (boardIdent[r][c] != 0)
                {
                    Coord mapPositioin = {r,c};
                    Vector2f gamePosition = MapToGame(mapPositioin);
                    auto newHex = makeChild<Piece>((Piece::Type)Random::range(0,5), r, c);
                    newHex->transform.setPosition(gamePosition);
                }
            }
        }
    }

    struct Coord {
        int r;
        int c;
    };

    Coord GameToMap(Vector2f gamePosition)
    {
        float x = gamePosition.x;
        float y = gamePosition.y;
        float r = 2.f / 3.f * x / hexW - y / hexH + rCenter;
        float c = 4.f / 3.f * x / hexW + cCenter;
        return { (int)Math::round(r), (int)Math::round(c) };
    }

    Vector2f MapToGame(Coord mapPosition)
    {
        int r = mapPosition.r;
        int c = mapPosition.c;
        float x = 3.f / 4.f * hexW * (c - cCenter);
        float y = hexH * (1.f / 2.f * (c - cCenter) + (rCenter - r));
        Vector2f gamePosition(x, y);
        return gamePosition;
    }


};

int main(int argc, char const *argv[])
{
    Engine::init(900,850,"Hexic");
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<Board>();
    Engine::run();
    return 0;
}

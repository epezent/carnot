#include <carnot>

using namespace carnot;

class FileDropDemo : public GameObject {
public:

    void start() {
        Engine::onFileDrop.connect(this, &FileDropDemo::onFileDrop);
    }

    void onFileDrop(const std::string& filePath, const Vector2u pos) {
        std::cout << "\"" << filePath << "\" dropped at " << pos << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500,"File Drop Demo");
    Engine::makeRoot<FileDropDemo>();
    Engine::run();
    return 0;
}

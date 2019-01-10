#pragma once

#include <memory>
#include <SFVG/Imports.hpp>
#include <SFVG/Print.hpp>
#include <SFVG/Random.hpp>
#include <SFVG/Engine/ResourceManager.hpp>
#include <SFVG/Engine/Input.hpp>
#include <SFVG/Engine/Handle.hpp>
#include <SFVG/Engine/Id.hpp>
#include <SFVG/Engine/System.hpp>
#include <SFVG/Engine/Object.hpp>

namespace sfvg {

class Engine : private NonCopyable {
public:

    //=========================================================================
    // GENERAL
    //=========================================================================

    /// Constructor
    Engine(unsigned int windowWidth, unsigned int windowHeight, unsigned int style = WindowStyle::Close);
    /// Destructor
    ~Engine();

    /// Starts running the Engine
    void run();
    /// Shows basic engine info
    void showInfo(bool show);

    //=========================================================================
    // RENDERING
    //=========================================================================

    /// Gets an Engine view (i.e. a camera)
    View& getView(std::size_t index);
    /// Add a new view to the Engine
    void addView();
    /// Returns the current size of the Window in world units
    Vector2f getWorldSize() const;
    /// Set color Window is cleared with
    void setBackgroundColor(const Color& color);
    /// Set the number of layers drawn by the Engine (default 1)
    void setLayerCount(std::size_t count);
    /// Get the number of layers drawn by the Engine (default 1)
    std::size_t getLayerCount() const;

    //=========================================================================
    // ROOT OBJECT
    //=========================================================================

    /// Makes a root Object of a specifc type and returns a handle to it
    template <typename T, typename ...Args> Handle<T> makeRoot(Args... args);
    /// Sets the root Object of the Engine
    void setRoot(Ptr<Object> root);
    /// Gets a Handle to the root Object of the Engine
    Handle<Object> getRoot() const;

public:

    /// Returns the current Engine time
    static float time();
    /// Returns the elapsed time since the last frame update
    static float deltaTime();

private:
    void processEvents();
    void update();
    void render();
    void updateStats();

public:

    RenderWindow window;
    ResourceManager<Texture, std::string> textures;
    ResourceManager<Font,    std::string> fonts;

private:
    Ptr<Object> m_root;
    std::vector<View> m_views;
    RenderQue m_renderQue;

    Font m_font;
    bool m_showTitleBar;
    Text m_infoText;
    bool m_showInfo;
    Color m_backgroundColor;
};

//==============================================================================
// Template / Inline Function Implementations
//==============================================================================

template <typename T, typename ...Args>
Handle<T> Engine::makeRoot(Args... args) {
    auto root = Object::make<T>(args...);
    setRoot(root);
    return getRoot().as<T>();
}

} // namespace sfvg

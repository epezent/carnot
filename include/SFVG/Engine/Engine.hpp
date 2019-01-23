#pragma once

#include <memory>
#include <SFVG/Imports.hpp>
#include <SFVG/Print.hpp>
#include <SFVG/Random.hpp>
#include <SFVG/Engine/ResourceManager.hpp>
#include <SFVG/Engine/Systems/InputSystem.hpp>
#include <SFVG/Engine/Systems/PhysicsSystem.hpp>
#include <SFVG/Engine/Handle.hpp>
#include <SFVG/Engine/Id.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Component.hpp>

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

    /// Returns the current Engine time
    float time();
    /// Returns the elapsed time since the last frame update
    float deltaTime();

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
    void setRoot(Ptr<GameObject> root);
    /// Gets a Handle to the root Object of the Engine
    Handle<GameObject> getRoot() const;


private:
    void processEvents();
    void render();
    void updateStats();

public:

    RenderWindow window;
    ResourceManager<Texture, std::string> textures;
    ResourceManager<Font,    std::string> fonts;
    InputSystem input;
    PhysicsSystem physics;

private:
    Ptr<GameObject> m_root;
    std::vector<View> m_views;
    RenderQue m_renderQue;

    Text m_infoText;
    Text m_pauseText;

    bool m_showInfo;
    Color m_backgroundColor;

    Clock m_clock;
    float m_timeValue;
    float m_deltaTimeValue;

    bool m_paused;
    bool m_advance;
};

//==============================================================================
// Template / Inline Function Implementations
//==============================================================================

template <typename T, typename ...Args>
Handle<T> Engine::makeRoot(Args... args) {
    auto root = Object::make<T>(*this, args...);
    setRoot(root);
    return getRoot().as<T>();
}

} // namespace sfvg

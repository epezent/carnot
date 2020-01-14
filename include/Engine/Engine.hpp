#pragma once

#include <Utility/Types.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/DebugSystem.hpp>
#include <Engine/InputSystem.hpp>
#include <Physics/PhysicsSystem.hpp>

namespace carnot {
class Engine { 
public:   

//=============================================================================
// GENERAL
//=============================================================================

/// Initialize Engine (fullscreen style)
static void init(const std::string& title = "");
/// Initialize Engine (default window style)
static void init(unsigned int windowWidth, unsigned int windowHeight, const std::string& title = "");
/// Initialize Engine (custom window style)
static void init(unsigned int windowWidth, unsigned int windowHeight, unsigned int style, const std::string& title = "");

/// Starts running the Engine
static void run();
/// Stops running the Engine
static void stop();
/// Returns the current Engine time
static float time();
/// Returns the elapsed time since the last frame update
static float deltaTime();
/// Returns the current frame number
static std::size_t frame();

//=============================================================================
// RENDERING
//=============================================================================

/// Gets an Engine view (i.e. a camera)
static View& getView(std::size_t index);
/// Add a new view to the Engine
static void addView();
/// Returns the current size of the Window in world units
static Vector2f getWorldSize();
/// Set color Window is cleared with
static void setBackgroundColor(const Color& color);
/// Set the number of layers drawn by the Engine (default 1)
static void setLayerCount(std::size_t count);
/// Get the number of layers drawn by the Engine (default 1)
static std::size_t getLayerCount();
/// Get the DPI scaling factor
static float getDpiFactor();
/// Get the realtime window size
static Vector2u getWindowSize();

//=============================================================================
// ROOT OBJECT
//=============================================================================

/// Makes a root Object of a specifc type and returns a handle to it
template <typename T, typename ...Args> static Handle<T> makeRoot(Args... args);
/// Sets the root Object of the Engine
static void setRoot(Ptr<GameObject> root);
/// Gets a Handle to the root Object of the Engine
static Handle<GameObject> getRoot();

//=============================================================================
// GLOBAL RESOURCES
//=============================================================================

static Signal<void(const std::string&, const Vector2u&)> onFileDrop;

//=============================================================================
// GLOBAL RESOURCES
//=============================================================================

static Ptr<RenderWindow> window;  ///< Engine render window

static ResourceManager<Texture>     textures; ///< 2D textures
static ResourceManager<Font>        fonts;    ///< fonts
// static ResourceManager<SoundBuffer> sounds;   ///< SFX/music
static ResourceManager<Shader>      shaders;  ///< shaders

private:

    static void eventThread();
    static void renderThread();
    static void render();
    static void processEvents();

};

//==============================================================================
// Template / Inline Function Implementations
//==============================================================================

template <typename T, typename ...Args>
Handle<T> Engine::makeRoot(Args... args) {
    auto root = make<T>(args...);
    setRoot(root);
    return getRoot().as<T>();
}

} // namespace carnot
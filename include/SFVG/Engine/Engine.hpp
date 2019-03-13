#pragma once

#include <SFVG/Common/Imports.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/ResourceManager.hpp>
#include <SFVG/Engine/Systems/DebugSystem.hpp>
#include <SFVG/Engine/Systems/InputSystem.hpp>
#include <SFVG/Engine/Systems/PhysicsSystem.hpp>

namespace sfvg {
class Engine { 
public:   

//=============================================================================
// GENERAL
//=============================================================================

/// Initialize Engine (Fullscreen)
static void init();
/// Initialize Engine (Windowed Mode)
static void init(unsigned int windowWidth, unsigned int windowHeight, unsigned int style = WindowStyle::Default);

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

static Ptr<RenderWindow> window;  ///< Engine render window

static ResourceManager<Texture, std::string>     textures; ///< 2D textures
static ResourceManager<Font, std::string>        fonts;    ///< fonts
static ResourceManager<SoundBuffer, std::string> sounds;   ///< SFX/music
static ResourceManager<Shader, std::string>      shaders;  ///< shaders

private:

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

} // namespace sfvg

//==============================================================================
// MACROS
//==============================================================================

/// Macro to request high performance GPU in systems (usually laptops) with both
/// dedicated and discrete GPUs
#ifdef _WIN32
    #define SFVG_USE_DISCRETE_GPU \
        extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1; \
        extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
#else
    #define SFVG_USE_DISCRETE_GPU
#endif

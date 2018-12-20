#pragma once

#include <memory>
#include <SFVG/Engine/Imports.hpp>
#include <SFVG/Engine/ResourceManager.hpp>
#include <SFVG/Engine/Clock.hpp>
#include <SFVG/Engine/Input.hpp>
#include <SFVG/Engine/Print.hpp>
#include <SFVG/Engine/Random.hpp>
#include <SFVG/Engine/Layer.hpp>
#include <SFVG/Engine/Handle.hpp>
#include <SFVG/Engine/Id.hpp>
#include <SFVG/Engine/System.hpp>

class Object;

typedef std::vector<std::vector<std::pair<const Object*, RenderStates>>> RenderQue;

class Engine : private NonCopyable {
public:
    Engine();
    /// Sets the root Object of the Engine
    void setRoot(Ptr<Object> root);
    /// Gets a Handle to the root Object of the Engine
    Handle<Object> getRoot() const;
    /// Starts running the Engine
    void run();
    /// Sets the window size of the Engine
    void setWindowSize(const Vector2f& windowSize);
    /// Sets the window title
    void setWindowTitle(const std::string& name);
    void showMouseCursor(bool show);
    void setBackgroundColor(const sf::Color& color);
    ///
    void setScaleFactor(float scaleFactor);
    /// Return window size in pixels
    sf::Vector2f getWindowSize() const;
    /// Return globel size of Engine
    sf::Vector2f getGlobalSize() const;

    /// Shows engine debug info
    void showDebugInfo(bool show);

    /// Set the number of layers drawn by the Engine (default 1)
    void setLayerCount(std::size_t count);
    /// Get the number of layers drawn by the Engine (default 1)
    std::size_t getLayerCount() const;

private:
    void initWindow();
    void processEvents();
    void update();
    void render();
    void updateStats();

private:
    Ptr<Object> m_root;
    RenderQue m_renderQue;
    RenderWindow m_window;
    Vector2f m_windowSize;
    std::string m_appName;
    View m_view;
    Font m_font;
    Text m_debugText;
    bool m_showDebug;
    float m_scaleFactor;
    Color m_backgroundColor;
};

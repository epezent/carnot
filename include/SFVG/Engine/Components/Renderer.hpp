#pragma once

#include <SFVG/Imports.hpp>
#include <SFVG/Graphics.hpp>
#include <SFVG/Engine/Component.hpp>

namespace sfvg {

class Renderer : public Component {
public:

    /// Constructor
    Renderer(GameObject& gameObject);
    /// Destructor
    ~Renderer();
    /// Set the render layer of the GameObject
    void setLayer(std::size_t layer);
    /// Get the render layer of the GameObject
    std::size_t getLayer() const;
    /// Increments render layer until Engine layer count reached
    void incrementLayer();
    /// Decrements render layer until Engine layer 0 reached
    void decrementLayer();
    /// Sets the render layer to the bottom layer
    void sendToBack();
    /// Sets the render layer to the top layer
    void sendToFront();

public:

    /// Gets the number of Renderers alive
    static std::size_t getRendererCount();

protected:

    friend class Engine;

    /// Ques this Renderer
    void onRender(RenderQue& que) override;
    /// Must be overriden to draw the Renderer
    virtual void render(RenderTarget& target) const = 0;

protected:

    mutable RenderStates m_states;  ///< RenderStates

private:

    std::size_t m_layer;   ///< the render layer

};

} // namespace sfvg

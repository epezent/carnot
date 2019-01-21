#pragma once

#include <SFVG/Imports.hpp>
#include <SFVG/Graphics.hpp>
#include <SFVG/Engine/Component.hpp>

namespace sfvg {

class RendererBase : public Component {
public:

    /// Constructor
    RendererBase(GameObject& gameObject);
    /// Destructor
    ~RendererBase();

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

    static std::size_t getRendererCount();

protected:

    friend class Engine;

    /// Ques this Renderer
    void onRender(RenderQue& que) override;

    /// Must be overriden to draw the Renderer
    virtual void render(RenderTarget& target) const = 0;

private:

    std::size_t m_layer;   ///< the render layer of the GameObject

};

template <class TDrawable>
class Renderer : public RendererBase {
public:

    Renderer(GameObject& gameObject) :
        RendererBase(gameObject),
        m_states(RenderStates::Default)
    { }

    TDrawable drawable;

protected:

    virtual void render(RenderTarget& target) const override {
        m_states.transform = gameObject.transform.getGlobalMatrix();
        target.draw(drawable, m_states);
    }

private:

    mutable RenderStates m_states;
};

typedef Renderer<Sprite> SpriteRenderer;
typedef Renderer<Shape>  ShapeRenderer;
typedef Renderer<Text>   TextRenderer;

} // namespace sfvg

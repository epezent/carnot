#pragma once

#include <SFVG/Engine/Components/Collider.hpp>

namespace sfvg {

class BoxCollider : public Collider {
public:

    /// Constructor
    BoxCollider(GameObject& gameObject,
                float width, float height, float radius = 0.0f);

private:

    void update() override;

};

} // namespace sfvg



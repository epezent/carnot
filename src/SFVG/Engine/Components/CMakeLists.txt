target_sources(sfvg
	PRIVATE
        Renderer.cpp
        RigidBody.cpp
        ShapeRenderer.cpp
        SpriteRenderer.cpp
        TextRenderer.cpp
		Transform.cpp
)

add_subdirectory(Detail)

target_sources(sfvg
	PRIVATE
        LineRenderer.cpp
        Renderer.cpp
        RigidBody.cpp
        ShapeRenderer.cpp
        SpriteRenderer.cpp
        TextRenderer.cpp
		Transform.cpp
)

add_subdirectory(Detail)

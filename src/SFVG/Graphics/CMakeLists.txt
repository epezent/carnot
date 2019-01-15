target_sources(sfvg
	PRIVATE
		Anchor.cpp
		Color.cpp
		Gradient.cpp
		Path.cpp
		Points.cpp
		Shape.cpp
		SharedResources.cpp
		SharedResources.hpp
		Stroke.cpp
)

add_subdirectory(Detail)
add_subdirectory(Shapes)
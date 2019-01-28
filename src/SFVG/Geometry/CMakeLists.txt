target_sources(sfvg
  PRIVATE
      Anchor.cpp
      CircleShape.cpp
      CrossShape.cpp
      Path.cpp
      Points.cpp
      PolygonShape.cpp
      RectangleShape.cpp
      Shape.cpp
      SquareShape.cpp
      StarShape.cpp
      Stroke.cpp
)

# add the subdirectories to get source files
add_subdirectory(Detail)

add_executable(engine src/example_engine.cpp)
target_link_libraries(engine SFVG::sfvg)

add_executable(pong src/example_pong.cpp)
target_link_libraries(pong SFVG::sfvg)

add_executable(shapes src/example_shapes.cpp)
target_link_libraries(shapes SFVG::sfvg)

add_executable(tween src/example_tween.cpp)
target_link_libraries(tween SFVG::sfvg)
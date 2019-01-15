
macro(sfvg_example SFVG_EXAMPLE_NAME)
    project(${SFVG_EXAMPLE_NAME})
    add_executable(${SFVG_EXAMPLE_NAME} "example_${SFVG_EXAMPLE_NAME}.cpp")
    target_link_libraries(${SFVG_EXAMPLE_NAME} chipmunk ws2_32 winmm Pdh Psapi XInput sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)
    set_target_properties(${SFVG_EXAMPLE_NAME} PROPERTIES FOLDER "Examples")
endmacro()


# sfvg_example(color)
# sfvg_example(coroutine)
# sfvg_example(online)
# sfvg_example(physics)
# sfvg_example(pong)
# sfvg_example(shapes)
# sfvg_example(tween)

add_executable(engine src/example_engine.cpp)
target_link_libraries(engine SFVG::sfvg)

add_executable(pong src/example_pong.cpp)
target_link_libraries(pong SFVG::sfvg)

add_executable(shapes src/example_shapes.cpp)
target_link_libraries(shapes SFVG::sfvg)

add_executable(tween src/example_tween.cpp)
target_link_libraries(tween SFVG::sfvg)


target_include_directories(carnot PRIVATE ../../include/ImGui)
target_sources(carnot
    PRIVATE
        imgui-SFML.cpp
        imgui.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
)

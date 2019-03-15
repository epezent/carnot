target_sources(carnot
	PRIVATE
		Component.cpp
		Coroutine.cpp
		Engine.cpp
		GameObject.cpp
		Id.cpp
		Object.cpp
		ProcessInfo.cpp
		XboxController.cpp
)

add_subdirectory(Components)
add_subdirectory(Fonts)
add_subdirectory(Systems)
add_subdirectory(ImGui)

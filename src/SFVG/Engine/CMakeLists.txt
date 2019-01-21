target_sources(sfvg
	PRIVATE
		Component.cpp
		Coroutine.cpp
		Engine.cpp
		Id.cpp
		Object.cpp
        GameObject.cpp
		ProcessInfo.cpp
        System.cpp
		XboxController.cpp
)

add_subdirectory(Components)
add_subdirectory(Fonts)
add_subdirectory(Systems)

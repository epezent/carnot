target_sources(sfvg
	PRIVATE
		Print.cpp
		Random.cpp
)

# add the subdirectories to get source files
add_subdirectory(Engine)
add_subdirectory(Graphics)
add_subdirectory(Animation)

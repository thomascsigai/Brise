add_executable(brise_sandbox
	main.cpp
)

find_package(SDL3 REQUIRED)

target_link_libraries(brise_sandbox PRIVATE brise SDL3::SDL3)
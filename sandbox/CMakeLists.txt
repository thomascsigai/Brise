# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

add_executable(brise_sandbox main.cpp "demos/springs.h" "demos/buoyancy.h"  "demos/collision.h" "demos/resting.h" "demos/cables.h")

target_include_directories(brise_sandbox PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/demos
)

target_link_libraries(brise_sandbox PRIVATE SDL3::SDL3 brise)
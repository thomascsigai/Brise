cmake_minimum_required(VERSION 3.16)

project(
	Brise
	LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Change option here if you want to build or not the sandbox by default
option(BRISE_BUILD_SANDBOX "Build the Brise sandbox application" ON)

add_library(
	brise STATIC
	src/Particle.cpp
	src/Vec2.cpp
	src/PForceGen.cpp
	src/World.cpp
	src/PContact.cpp
)

target_include_directories(brise PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (BRISE_BUILD_SANDBOX)
	add_subdirectory(sandbox)
endif()
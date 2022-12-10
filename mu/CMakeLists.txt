include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")
add_library(MU ${SOURCES})

add_subdirectory(libs/lua-5.4.4)
list(APPEND EXTRA_LIBS LuaLib)

add_subdirectory("libs/glad")
list(APPEND EXTRA_LIBS GLAD)

add_subdirectory(libs/imgui)
list(APPEND EXTRA_LIBS IMGUI)

#GLFW
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("libs/glfw-3.3.8")
list(APPEND EXTRA_LIBS glfw)

project(MU VERSION 1.0)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

target_link_libraries(MU PUBLIC ${EXTRA_LIBS})
target_include_directories(MU
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


install(TARGETS MU DESTINATION lib)
install(DIRECTORY include DESTINATION include)
cmake_minimum_required(VERSION 3.10)

project(MU VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in ${PROJECT_SOURCE_DIR}/include/config.h)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")


add_subdirectory(FRACTAL)
add_subdirectory(libs/lua-5.4.4)

#add_subdirectory(lua-5.4.4)
list(APPEND EXTRA_LIBS FRACTAL)
list(APPEND EXTRA_LIBS LuaLib)

add_executable(MU ${SOURCES})

target_link_libraries(MU PUBLIC ${EXTRA_LIBS})
#target_link_libraries(MU PUBLIC lua-5.4.4)
#target_link_libraries(MU PUBLIC LuaLib)

target_include_directories(MU PUBLIC "${PROJECT_BINARY_DIR}/include")

install(TARGETS MU DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/include/config.h"
        DESTINATION include)

include(CTest)
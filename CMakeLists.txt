cmake_minimum_required(VERSION 3.10)

project(MU VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in ${PROJECT_SOURCE_DIR}/include/config.h)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_subdirectory(POLARIS)
add_subdirectory(FRACTAL)
list(APPEND EXTRA_LIBS POLARIS)
list(APPEND EXTRA_LIBS FRACTAL)

add_executable(MU ${SOURCES})

target_link_libraries(MU PUBLIC ${EXTRA_LIBS})

target_include_directories(MU PUBLIC "${PROJECT_BINARY_DIR}/include")

install(TARGETS MU DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/include/config.h"
        DESTINATION include)

include(CTest)
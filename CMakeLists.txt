cmake_minimum_required(VERSION 3.10)

project(MU_APP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_subdirectory(MU)

list(APPEND EXTRA_LIBS MU)

add_executable(MU_APP ${SOURCES})

target_link_libraries(MU_APP PUBLIC ${EXTRA_LIBS})
target_include_directories(MU_APP PUBLIC "${PROJECT_BINARY_DIR}/include")

install(TARGETS MU_APP DESTINATION bin)

include(CTest)
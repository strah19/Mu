include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_library(FRACTAL ${SOURCES})

target_include_directories(FRACTAL
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS FRACTAL DESTINATION lib)
install(DIRECTORY include DESTINATION include)
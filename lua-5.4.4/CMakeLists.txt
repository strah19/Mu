include_directories(src)

file(GLOB SOURCES "src/*.c")
add_library(LUA ${SOURCES})

target_include_directories(LUA
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS LUA DESTINATION lib)
install(DIRECTORY src DESTINATION src)
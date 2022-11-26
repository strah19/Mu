include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_library(POLARIS ${SOURCES})

add_subdirectory(VM)
target_link_libraries(POLARIS PUBLIC VM)

target_include_directories(POLARIS
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS POLARIS DESTINATION lib)
install(DIRECTORY include DESTINATION include)
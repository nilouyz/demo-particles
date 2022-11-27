include_directories(
    "./thirdParty/"
)
link_directories("${CMAKE_SOURCE_DIR}/libs/")

project(demo)
aux_source_directory("demo" demo)
add_executable(demo ${demo})
target_link_libraries(demo glew32.dll glfw3.dll opengl32.lib)
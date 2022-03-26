cmake_minimum_required(VERSION 3.15)
project(RPG VERSION 0.0.5 LANGUAGES C CXX)
 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
target_link_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)

add_subdirectory(libs/glad)

if(NOT TRUERPG_USE_SYSTEM_GLFW)
    add_subdirectory(libs/glfw)
else()
    find_package(glfw3 3.3 REQUIRED)
endif()

find_package(OpenGL REQUIRED)
add_subdirectory(libs/stb_image)
add_subdirectory(libs/glm)
add_subdirectory(libs/freetype)
add_subdirectory(libs/miniaudio)
add_subdirectory(libs/entt)

set(PROJECT_LIBS glad glfw OpenGL::GL stb_image glm freetype miniaudio entt)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

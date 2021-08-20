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

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
find_package(OpenGL REQUIRED)
add_subdirectory(libs/stb_image)
add_subdirectory(libs/glm)
add_subdirectory(libs/freetype)
add_subdirectory(libs/miniaudio)
add_subdirectory(libs/sol2)

set(PROJECT_LIBS glad glfw OpenGL::GL stb_image glm freetype miniaudio sol2_single)

if(UNIX AND NOT APPLE)
    message(STATUS "Its linux")
    set(PROJECT_LIBS ${PROJECT_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/libs/luaJIT/linux/x64/libluajit.a)
elseif(MINGW)
    message(STATUS "Its MinGW")
    set(PROJECT_LIBS ${PROJECT_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/libs/luaJIT/mingw64/lua51.dll)
elseif(WIN32)
    message(STATUS "Its Windows")
    set(PROJECT_LIBS ${PROJECT_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/libs/luaJIT/windows/x86/lua51.dll)
endif()


target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC libs/entt/include libs/luaJIT/include)

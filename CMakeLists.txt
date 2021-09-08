cmake_minimum_required(VERSION 3.15)
project(TrueRPG VERSION 0.0.5 LANGUAGES C CXX)

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

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "${PROJECT_NAME} only supports x64 platforms")
endif()

message("X64 platform")

# LuaJIT 2.0.5
if(UNIX AND NOT APPLE)
    message("Linux")
    set(LUA_JIT ${PROJECT_SOURCE_DIR}/libs/luaJIT/linux/libluajit.a)
elseif(WIN32)
    if(MSVC)
        message("MSVC")
        set(LUA_JIT ${PROJECT_SOURCE_DIR}/libs/luaJIT/msvc/lua51.lib)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/libs/luaJIT/msvc/lua51.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    elseif(MINGW)
        message("MinGW")
        target_compile_options(${PROJECT_NAME} PUBLIC -O1 -Wa,-mbig-obj)
        set(LUA_JIT ${PROJECT_SOURCE_DIR}/libs/luaJIT/mingw/lua51.dll)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/libs/luaJIT/mingw/lua51.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else()
        message(FATAL_ERROR "Unsupported compiler")
    endif()
else()
    message(FATAL_ERROR "Unsupported OS")
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBS} ${LUA_JIT})

target_include_directories(${PROJECT_NAME} PRIVATE libs/entt/include libs/luaJIT/include)

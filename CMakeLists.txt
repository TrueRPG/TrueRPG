cmake_minimum_required(VERSION 3.15)
project(RPG VERSION 0.0.5 LANGUAGES C CXX)
 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

option(TRUERPG_USE_SYSTEM_GLFW "Use system GLFW installation" OFF)
option(TRUERPG_USE_SYSTEM_GLM "Use system GLM installation" OFF)
option(TRUERPG_USE_SYSTEM_FREETYPE "Use system freetype installation" OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#if(NOT WIN32)
#    # it doesn't work on Windows with mingw
#    target_compile_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
#    target_link_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
#endif()

add_subdirectory(libs/glad)

if(NOT TRUERPG_USE_SYSTEM_GLFW)
  add_subdirectory(libs/glfw)
else()
  find_package(glfw3 REQUIRED)
endif()

find_package(OpenGL REQUIRED)
add_subdirectory(libs/stb_image)

if(NOT TRUERPG_USE_SYSTEM_GLM)
  add_subdirectory(libs/glm)
else()
  find_package(glm REQUIRED)
endif()

if(NOT TRUERPG_USE_SYSTEM_FREETYPE)
  add_subdirectory(libs/freetype)
else()
  find_package(Freetype REQUIRED)
  target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
endif()

add_subdirectory(libs/miniaudio)
add_subdirectory(libs/entt)

option(YAML_CPP_BUILD_TOOLS OFF)
option(YAML_CPP_BUILD_TESTS OFF)
add_subdirectory(libs/yaml-cpp)

set(PROJECT_LIBS glad glfw OpenGL::GL stb_image freetype miniaudio entt yaml-cpp)
if(NOT TRUERPG_USE_SYSTEM_GLM)
  set(PROJECT_LIBS ${PROJECT_LIBS} glm)
endif()

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
install(TARGETS ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.16)
project(vma)

add_library(${PROJECT_NAME} STATIC src/vk_mem_alloc.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
cmake_minimum_required(VERSION 4.0)
project(raytracer)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)

target_include_directories(raytracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenMP COMPONENTS CXX)
if(OpenMP_CXX_FOUND)
    target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(raytracer PRIVATE "USE_OPENMP")
endif()
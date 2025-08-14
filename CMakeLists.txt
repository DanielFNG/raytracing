cmake_minimum_required(VERSION 4.0)
project(raytracer)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion -Weffc++)
#add_compile_options(-O2 -march=native)

# No significant difference from the below, so far anyway
#set(CMAKE_CXX_FLAGS "-O2 -march=native")

add_subdirectory(src)

target_include_directories(raytracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenMP COMPONENTS CXX)
if(OpenMP_CXX_FOUND)
    target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(raytracer PRIVATE "USE_OPENMP")
endif()
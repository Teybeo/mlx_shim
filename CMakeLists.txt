cmake_minimum_required(VERSION 3.13)
project(mlx_shim C)

set(CMAKE_C_STANDARD 99)

set(SOURCE src/lib.c)

add_library(mlx_shim STATIC ${SOURCE})

target_include_directories(mlx_shim PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

set_target_properties(mlx_shim
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib"
    )


# Add SDL2 path to list of searched paths for packages
set(CMAKE_PREFIX_PATH "D:/Dev/Lib/SDL2-2.0.8")

find_package(SDL2 REQUIRED)

# Stay compatible with old style find_package results
if (NOT TARGET SDL2::SDL2)
    target_link_libraries(mlx_shim ${SDL2_LIBRARIES})
    target_include_directories(mlx_shim PRIVATE ${SDL2_INCLUDE_DIRS})
else()
    target_link_libraries(mlx_shim SDL2::SDL2)
endif()

target_link_libraries(mlx_shim core)

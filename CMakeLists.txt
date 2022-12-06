project(incremental_shortest_path)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/nanoflann)
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${PROJECT_SOURCE_DIR}/thirdparty/nanoflann/include)
add_library(map SHARED map.cpp map_generator.cpp solver.cpp)
add_executable(run_main main.cpp)
target_link_libraries(run_main map nanoflann)
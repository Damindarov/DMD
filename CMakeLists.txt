cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# we need to use C++20 for MeshLib
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dmd_project CXX)

find_package(MeshLib CONFIG REQUIRED)
find_package(TBB REQUIRED)

include_directories(include)

add_executable(meshlib_boolean_pipeline src/boolean_pipeline.cpp)
target_include_directories(meshlib_boolean_pipeline PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(meshlib_boolean_pipeline PRIVATE MeshLib::MRMesh MeshLib::MRVoxels TBB::tbb)
target_link_directories(meshlib_boolean_pipeline PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})


add_executable(meshlib_global_local_icp src/global_local_icp.cpp)
target_include_directories(meshlib_global_local_icp PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(meshlib_global_local_icp PRIVATE MeshLib::MRMesh MeshLib::MRVoxels TBB::tbb)
target_link_directories(meshlib_global_local_icp PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})


add_executable(meshlib_simple_boolean src/simple_boolean.cpp)
target_include_directories(meshlib_simple_boolean PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(meshlib_simple_boolean PRIVATE MeshLib::MRMesh MeshLib::MRVoxels TBB::tbb)
target_link_directories(meshlib_simple_boolean PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})


add_executable(meshlib_main src/main.cpp
                            include/Pipeline.h
                            src/Pipeline.cpp)
target_include_directories(meshlib_main PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(meshlib_main PRIVATE MeshLib::MRMesh MeshLib::MRVoxels TBB::tbb)
target_link_directories(meshlib_main PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-examples CXX)

find_package(MeshLib CONFIG REQUIRED)
find_package(TBB REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

set(TARGET test)

set(SOURCES
  simple_application.cpp
)

add_executable(${TARGET} ${SOURCES})
target_compile_definitions(${TARGET} PRIVATE SPDLOG_FMT_EXTERNAL)
target_include_directories(${TARGET} PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE MeshLib::MRMesh MeshLib::MRViewer TBB::tbb fmt::fmt spdlog::spdlog)
target_link_directories(${TARGET} PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})


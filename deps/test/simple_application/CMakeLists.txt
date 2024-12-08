 cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-examples CXX)

find_package(MeshLib CONFIG REQUIRED)
find_package(TBB REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)


find_package(jsoncpp REQUIRED)

set(TARGET1 example_plugin)

set(SOURCES1 
	MyPlugin.cpp			MyPlugin.hpp
	holefiller.cpp			holefiller.hpp
	local_icp.cpp 			local_icp.hpp
	helper_functions.cpp	helper_functions.hpp
	mystatetool.cpp 		mystatetool.hpp)

file(GLOB UI_JSON "*.ui.json")
file(GLOB ITEMS_JSON "*.items.json")

set(MESHLIB_SHARE /usr/local/share/MeshLib)
set(UI_JSON_DESTINATION ${MESHLIB_SHARE})
set(ITEMS_JSON_DESTINATION ${MESHLIB_SHARE})
set(RESOURCE_FOLDER "resource")
set(RESOURCE_DESTINATION ${MESHLIB_SHARE})

add_library(${TARGET1} SHARED ${SOURCES1})

#target_include_directories(${TARGET1} PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR})
target_link_libraries(${TARGET1} PUBLIC
	TBB::tbb
	MeshLib::MRMesh
	MeshLib::MRViewer
	glad
	glfw
	imgui
	jsoncpp_lib
	spdlog
	)
target_link_directories(${TARGET1} PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})
target_include_directories(${TARGET1} PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR} ${JSON_INC_PATH})



set(TARGET test)

set(SOURCES
  simple_application.cpp
)

add_executable(${TARGET} ${SOURCES})
target_compile_definitions(${TARGET} PRIVATE SPDLOG_FMT_EXTERNAL)
target_include_directories(${TARGET} PUBLIC ${MESHLIB_INCLUDE_DIR} ${MESHLIB_THIRDPARTY_INCLUDE_DIR} ${JSON_INC_PATH})
target_link_libraries(${TARGET} PRIVATE MeshLib::MRMesh MeshLib::MRViewer TBB::tbb fmt::fmt spdlog::spdlog ${TARGET1})  # link the example_plugin aswell
target_link_directories(${TARGET} PUBLIC ${MESHLIB_THIRDPARTY_LIB_DIR})

set(MR_MAIN_LIB_DIR /usr/local/lib/MeshLib)


install(TARGETS ${TARGET1}
		EXPORT ${TARGET1}
		LIBRARY DESTINATION "${MR_MAIN_LIB_DIR}")

# copy these json files and resource folder to this runtime output directory "/usr/local/share/MeshLib"
install(FILES "${UI_JSON}"
		DESTINATION ${UI_JSON_DESTINATION})

install(FILES "${ITEMS_JSON}"
		DESTINATION ${ITEMS_JSON_DESTINATION})

install(DIRECTORY "${RESOURCE_FOLDER}"
		DESTINATION ${RESOURCE_DESTINATION})

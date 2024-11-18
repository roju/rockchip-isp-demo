cmake_minimum_required(VERSION 2.8...3.22)

#project name
PROJECT(rkaiq_test)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fPIC")
set(CAMERA_ENGINE_RKAIQ_DIR ${PROJECT_SOURCE_DIR}/camera_engine_rkaiq)

# use librkaiq.so provided by system
# set(RKAIQ_LIBRARY_DIR /usr/lib)

# use librkaiq.so from local build
set(RKAIQ_LIBRARY_DIR ${CAMERA_ENGINE_RKAIQ_DIR}/build/rkaiq/all_lib/Release)

message("rkaiq src path: " ${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq)
message("rkaiq lib path: " ${RKAIQ_LIBRARY_DIR})

add_compile_options(-std=c++11)
add_compile_options(-DADD_RK_AIQ)
add_compile_options(-O)

SET(RK_AIQ_INCLUDE_DIR
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/uAPI2
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/xcore/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/algos/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/common/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/iq_parser/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/iq_parser_v2/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/uAPI2/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/common/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/xcore/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/algos/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/iq_parser/
	${CAMERA_ENGINE_RKAIQ_DIR}/rkaiq/include/iq_parser_v2/
)
SET(RK_AIQ_LIB rkaiq)

include_directories(
	${RK_AIQ_INCLUDE_DIR}
	include/common/mediactl
	./
	./common/
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
link_directories("${RKAIQ_LIBRARY_DIR}")
ADD_EXECUTABLE(rkaiq_test main.cpp common/mediactl/mediactl.c)
target_link_libraries(rkaiq_test ${RK_AIQ_LIB} pthread dl)

project(cwebrtc_thread_proxy)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "#########################")
MESSAGE(STATUS ${PROJECT_NAME})
ADD_DEFINITIONS(-std=c++11 -Wall -g)

INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}
)
file(GLOB ALL_C_SRCS
        ./*.cpp
		./*.cc
        )

add_executable(${PROJECT_NAME} ${ALL_C_SRCS})
target_link_libraries(${PROJECT_NAME})
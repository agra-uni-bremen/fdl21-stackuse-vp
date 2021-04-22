subdirs(rtos)

add_library(stacksize
	functions.cpp)

target_include_directories(stacksize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

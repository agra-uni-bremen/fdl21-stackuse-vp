subdirs(rtos)
subdirs(dwarf)

add_library(stacksize
	stacksize.cpp)

target_include_directories(stacksize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

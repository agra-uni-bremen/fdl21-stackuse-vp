add_library(rtos
	rtos.cpp
	riot.cpp)

target_include_directories(rtos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rtos PUBLIC dwarf)

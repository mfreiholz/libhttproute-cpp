cmake_minimum_required(VERSION 3.0)
project(libhttproute)
if(CMAKE_COMPILER_IS_GNUCC)
	# GCC 4.8.4 C++11
	#set(CMAKE_CXX_FLAGS "-std=c++11")
	# GCC 4.8.4 C++14
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
	# GCC 5.2.X C++14
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	# Strict flags
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
endif(CMAKE_COMPILER_IS_GNUCC)
set(headers
	defines.h
	http_server_request.h
	match.h
	route.h
	route_match.h
	route_handler.h
	route_matcher.h
	route_matcher_impl.h
	router.h
)
set(sources
	match.cpp
	route.cpp
	router.cpp
)
add_library(
	${PROJECT_NAME}
	STATIC
	${headers}
	${sources}
)

# unit test
if(WIN32)
	include_directories(../googletest/googletest/include)
	add_executable(
		${PROJECT_NAME}_test
		tests.cpp
	)
	target_link_libraries(
		${PROJECT_NAME}_test
		${PROJECT_NAME}
	)
endif(WIN32)
if(WIN32)
	target_link_libraries(
		${PROJECT_NAME}_test
		../../googletest/googletest/msvc/gtest/Debug/gtestd
	)
endif(WIN32)

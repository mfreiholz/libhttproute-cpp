cmake_minimum_required(VERSION 3.0)
project(libhttproute)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

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

# basic library sources
set(headers
	libhttproute/defines.h
	libhttproute/request.h
	libhttproute/request_impl.h
	libhttproute/match.h
	libhttproute/route.h
	libhttproute/route_match.h
	libhttproute/route_handler.h
	libhttproute/route_matcher.h
	libhttproute/route_matcher_impl.h
	libhttproute/router.h
)
set(sources
	libhttproute/match.cpp
	libhttproute/route.cpp
	libhttproute/router.cpp
)
add_library(
	${PROJECT_NAME}
	STATIC
	${headers}
	${sources}
)

# unit test
find_package(GoogleTest)
if(GOOGLETEST_FOUND)
	include_directories(${PROJECT_SOURCE_DIR})
	include_directories(${GOOGLETEST_INCLUDE_DIRS})
	add_executable(
		${PROJECT_NAME}_test
		test/tests.cpp
	)
	target_link_libraries(
		${PROJECT_NAME}_test
		${PROJECT_NAME}
		${GOOGLETEST_LIBRARY}
	)
endif(GOOGLETEST_FOUND)
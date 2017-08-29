cmake_minimum_required(VERSION 2.8)
project(libhttproute)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_EXTRA "")

#
# Options
#

option(BuildShared "BuildShared" OFF)
option(BuildTests "BuildTests" OFF)

if(BuildShared)
	set(StaticOrShared "SHARED")
	add_definitions(-DLIBHTTPROUTE_EXPORT)
else(BuildShared)
	set(StaticOrShared "STATIC")
endif(BuildShared)

if(WIN32)
	add_definitions(/Zc:wchar_t-)
	add_definitions(/INCREMENTAL:NO)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCC)
	# GCC 4.8.4 C++11
	#set(CMAKE_CXX_FLAGS "-std=c++11")
	# GCC 4.8.4 C++14
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
	# GCC 5.2.X C++14
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	# Strict flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif(CMAKE_COMPILER_IS_GNUCC)

#
# Sources
#

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
	libhttproute/request_impl.cpp
	libhttproute/match.cpp
	libhttproute/route.cpp
	libhttproute/router.cpp
)

#
# Target
#

add_library(
	${PROJECT_NAME}
	${StaticOrShared}
	${headers}
	${sources}
)

#
# Tests
#

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
find_package(GoogleTest)
if(GOOGLETEST_FOUND AND BuildTests)
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
endif(GOOGLETEST_FOUND AND BuildTests)
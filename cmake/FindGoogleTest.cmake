if(WIN32)
	find_path(
		GOOGLETEST_INCLUDE_DIRS
		NAMES
			gtest/gtest.h
		PATHS
			${PROJECT_SOURCE_DIR}/googletest/include/
			${PROJECT_SOURCE_DIR}/../googletest/include/
			${PROJECT_SOURCE_DIR}/../_vendor/googletest/build-vc14-md-x86-32-debug/include/
	)
	find_library(
		GOOGLETEST_LIBRARY
		NAMES
			gtestd
		HINTS
			${PROJECT_SOURCE_DIR}/googletest/lib/
			${PROJECT_SOURCE_DIR}/../googletest/lib/
			${PROJECT_SOURCE_DIR}/../_vendor/googletest/build-vc14-md-x86-32-debug/lib/
	)
endif()

if (GOOGLETEST_INCLUDE_DIRS AND GOOGLETEST_LIBRARY)
	set(GOOGLETEST_FOUND TRUE)
endif()
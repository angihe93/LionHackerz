cmake_minimum_required(VERSION 3.15)

project(LionHackerzProject)

set(CMAKE_CXX_STANDARD 17) #
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USED_PORT 18080)

set(GTEST_INCLUDE_DIRS /opt/homebrew/include)

set(INCLUDE_PATHS 
    include/
    external_libraries/boost_1_86_0
    external_libraries/WordNet-3.0/include
    external_libraries/Crow/include
    /opt/homebrew/Cellar/asio/1.30.2/include)

# Add coverage flags for GCC/Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage -fprofile-arcs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")

link_directories(external_libraries/WordNet-3.0/lib)

add_custom_target(unbind_port
    COMMAND lsof -t -i tcp:${USED_PORT} | xargs -r kill -9 || true
    COMMENT "Port ${USED_PORT} in use from earlier run.  Killing process before running."
)

add_custom_target(generate_coverage ALL
    COMMAND lcov --capture --directory . --output-file coverage.info --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range
    COMMAND lcov --remove coverage.info '*/googletest/*' '/usr/*' '*/test/*' --output-file coverage_filtered.info --ignore-errors inconsistent --ignore-errors range
    COMMAND genhtml coverage_filtered.info --output-directory coverage_report --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating coverage report"
)

# Main project executable
add_executable(LionHackerzProject 
    src/main.cpp 
    src/Database.cpp
    src/Matcher.cpp
    src/RouteController.cpp
    src/Listing.cpp
)

add_dependencies(LionHackerzProject unbind_port)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/ff233bdd4cac0a0bf6e5cd45bda3406814cb2796.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# set(BUILD_GMOCK ON CACHE BOOL "Build gmock")
FetchContent_MakeAvailable(googletest)

include(GoogleTest)


# Include directories for main executable
target_include_directories(LionHackerzProject PUBLIC ${INCLUDE_PATHS})

# Find and link with libcurl
find_package(CURL REQUIRED)
target_link_libraries(LionHackerzProject PRIVATE WN CURL::libcurl)

# Test executable
 add_executable(LionHackerzProjectTests
   test/testMain.cpp
   test/MatcherUnitTests.cpp
   src/Matcher.cpp
   src/Database.cpp
   src/Listing.cpp)

add_dependencies(LionHackerzProjectTests unbind_port generate_coverage)

find_package(GTest REQUIRED)

target_include_directories(LionHackerzProjectTests PRIVATE 
     ${INCLUDE_PATHS} 
     ${GTEST_INCLUDE_DIRS})

message(STATUS "GTest Include Dirs: ${GTEST_INCLUDE_DIRS}")

target_link_libraries(LionHackerzProjectTests PRIVATE 
     WN
     CURL::libcurl
     GTest::gtest 
     GTest::gtest_main
     pthread)

 enable_testing()
 gtest_discover_tests(LionHackerzProjectTests)

add_test(NAME LionHackerzProjectTests COMMAND LionHackerzProjectTests)
 # set_tests_properties(LionHackerzProjectTests PROPERTIES TIMEOUT 300)
 

option(COVERAGE "Enable coverage reporting" ON)
if(COVERAGE)
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Find the cpplint program
find_program(CPPLINT cpplint)

if (CPPLINT)
    # List of source files to be checked by cpplint
    set(SOURCE_FILES
        src/main.cpp 
        src/Database.cpp
        src/Matcher.cpp
        src/RouteController.cpp
        src/Listing.cpp
        test/MatcherUnitTests.cpp
    )

    # Custom target to run cpplint
    add_custom_target(
        cpplint
        COMMAND ${CPPLINT} --filter=-whitespace,-build/include_what_you_use ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cpplint for Google C++ Style Guide compliance"
    )
else()
    message(WARNING "cpplint not found! Skipping style checks.")
endif()

cmake_minimum_required(VERSION 3.15)

project(LionHackerzProject)

set(INCLUDE_PATHS include/ external_libraries/boost_1_86_0 external_libraries/WordNet-3.0/include external_libraries/Crow/include external_libraries/ $(GTEST_DIR)/googletest/include $(GTEST_DIR)/googlemock/include ${GTEST_INCLUDE_DIRS}/include) #
set(CMAKE_CXX_STANDARD 17) #
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add coverage flags for GCC/Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage") # -fprofile-arcs")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")

link_directories(external_libraries/WordNet-3.0/lib)

# Main project executable
add_executable(LionHackerzProject 
    src/main.cpp 
    src/Database.cpp
    src/Matcher.cpp
    src/RouteController.cpp
    # src/MyApp.cpp
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/ff233bdd4cac0a0bf6e5cd45bda3406814cb2796.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# set(BUILD_GMOCK ON CACHE BOOL "Build gmock")
FetchContent_MakeAvailable(googletest)

# Include directories for main executable
target_include_directories(LionHackerzProject PUBLIC 
    ${INCLUDE_PATHS} 
    include
    /opt/homebrew/Cellar/asio/1.30.2/include
)

# Find and link with libcurl
find_package(CURL REQUIRED)
target_link_libraries(LionHackerzProject PRIVATE 
    WN
    gtest 
    gtest_main
    CURL::libcurl  # Link with libcurl
    # gmock
    # gmock_main
)

enable_testing()

# Test executable
# add_executable(LionHackerzProjectTests
#   test/sample.cpp
#   test/CourseUnitTests.cpp
#   test/DepartmentUnitTests.cpp
#   test/MyFileDatabaseUnitTests.cpp
#   test/MyAppTests.cpp
    # test/MainTests.cpp
#   src/Course.cpp
#   src/Department.cpp
#   src/MyFileDatabase.cpp
#   src/MyApp.cpp
    # src/main.cpp
# )

# target_include_directories(LionHackerzProjectTests PRIVATE 
#     ${INCLUDE_PATHS} 
#     include
#     /opt/homebrew/Cellar/asio/1.30.2/include
#     ${gtest_SOURCE_DIR}/include 
#     # ${gmock_SOURCE_DIR}/include
# )

# target_link_libraries(LionHackerzProjectTests PRIVATE 
#     gtest 
#     gtest_main
# )

# include(GoogleTest)
# gtest_discover_tests(LionHackerzProjectTests)

# Find the cpplint program
find_program(CPPLINT cpplint)

if (CPPLINT)
    # List of source files to be checked by cpplint
    set(SOURCE_FILES
        src/main.cpp 
        src/Database.cpp
        src/Matcher.cpp
        src/RouteController.cpp
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

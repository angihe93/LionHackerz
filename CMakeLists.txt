cmake_minimum_required(VERSION 3.15)

project(LionHackerzProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the port used by the application
set(USED_PORT 18080)

# Initialize variables for include directories and library paths
set(INCLUDE_PATHS 
    include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external_libraries/Crow/include
    ${PROJECT_SOURCE_DIR}/external_libraries/WordNet-3.0/include
    /usr/local/include  # Corrected to absolute path
    external_libraries/boost_1_86_0
    external_libraries/openssl-3.3.2/include    
)

set(LINK_DIRECTORIES 
    /usr/local/lib
    ${PROJECT_SOURCE_DIR}/external_libraries/WordNet-3.0/lib
)

# Add additional include paths and link directories based on the OS
if(APPLE)
    # macOS-specific paths
    list(APPEND INCLUDE_PATHS 
        ${PROJECT_SOURCE_DIR}/external_libraries/boost_1_86_0
        ${PROJECT_SOURCE_DIR}/external_libraries/openssl-3.3.2/include
        /opt/homebrew/Cellar/asio/1.30.2/include
    )
    list(APPEND LINK_DIRECTORIES
        /opt/homebrew/lib
    )
elseif(UNIX)
    # Linux-specific paths (e.g., Docker)
    # Add any additional Linux-specific include or library paths here if needed
endif()

# Include directories for the project
include_directories(${INCLUDE_PATHS})

# Link directories for the project
link_directories(${LINK_DIRECTORIES})

# Add coverage flags for GCC/Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage -fprofile-arcs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")

# Custom target to free up the used port before running
add_custom_target(unbind_port
    COMMAND lsof -t -i tcp:${USED_PORT} | xargs -r kill -9 || true
    COMMENT "Port ${USED_PORT} in use from earlier run. Killing process before running."
)

# Main project executable
add_executable(LionHackerzProject 
    src/main.cpp 
    src/Database.cpp
    src/Matcher.cpp
    src/RouteController.cpp
    src/Listing.cpp
    src/User.cpp
    src/Augment.cpp
    src/Employer.cpp
    src/Auth.cpp
)

# Specify include directories for the target
target_include_directories(LionHackerzProject PRIVATE 
    include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external_libraries/Crow/include
    ${PROJECT_SOURCE_DIR}/external_libraries/WordNet-3.0/include
    /usr/local/include
    external_libraries/boost_1_86_0
    external_libraries/openssl-3.3.2/include
)

# Find and link CURL
find_package(CURL REQUIRED)
if(CURL_FOUND)
    target_include_directories(LionHackerzProject PRIVATE ${CURL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "CURL not found!")
endif()

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    target_include_directories(LionHackerzProject PRIVATE ${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found!")
endif()

# Find WordNet headers
find_path(WORDNET_INCLUDE_DIR wn.h
    PATHS 
        /usr/local/include 
        ${PROJECT_SOURCE_DIR}/external_libraries/WordNet-3.0/include
        /usr/local/WordNet-3.0/include
    NO_DEFAULT_PATH
    REQUIRED
)

if(NOT WORDNET_INCLUDE_DIR)
    message(FATAL_ERROR "Cannot find WordNet include directory containing wn.h")
endif()

# Find WordNet library
find_library(WORDNET_LIBRARY 
    NAMES WN 
    PATHS 
        /usr/local/lib 
        ${PROJECT_SOURCE_DIR}/external_libraries/WordNet-3.0/lib 
        /usr/local/WordNet-3.0/lib
    NO_DEFAULT_PATH
    REQUIRED
)

if(NOT WORDNET_INCLUDE_DIR)
    message(FATAL_ERROR "Cannot find WordNet include directory containing wn.h")
endif()

if(NOT WORDNET_LIBRARY)
    message(FATAL_ERROR "Cannot find WordNet library (wn or wordnet)")
endif()

# Include WordNet directories
target_include_directories(LionHackerzProject PRIVATE ${WORDNET_INCLUDE_DIR})

# Link WordNet library
target_link_libraries(LionHackerzProject PRIVATE 
    ${WORDNET_LIBRARY} 
    CURL::libcurl 
    OpenSSL::SSL 
    OpenSSL::Crypto
)

# Add dependency to ensure the port is freed before building
add_dependencies(LionHackerzProject unbind_port)

# Enable testing
enable_testing()

# Test executable
add_executable(LionHackerzProjectTests
    test/testMain.cpp
    test/MatcherUnitTests.cpp
    test/DatabaseUnitTests.cpp
    test/ListingUnitTests.cpp
    test/EmployerUnitTests.cpp
    src/Matcher.cpp
    src/Database.cpp
    src/Listing.cpp
    src/User.cpp
    src/Augment.cpp
    src/Employer.cpp
)

# Include directories for test executable
target_include_directories(LionHackerzProjectTests PUBLIC ${INCLUDE_PATHS})

# Link libraries for test executable
target_link_libraries(LionHackerzProjectTests PRIVATE 
     ${WORDNET_LIBRARY} 
     CURL::libcurl 
     GTest::gtest 
     GTest::gtest_main 
     pthread
)

# Conditional handling of googletest
if(APPLE)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/ff233bdd4cac0a0bf6e5cd45bda3406814cb2796.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
elseif(UNIX)
    # Use the package manager's googletest on Linux
    find_package(GTest REQUIRED)
    target_include_directories(LionHackerzProjectTests PRIVATE ${GTEST_INCLUDE_DIRS})
endif()

# Register tests
add_test(NAME LionHackerzProjectTests COMMAND LionHackerzProjectTests)

# Custom target to run tests
# Custom target to run tests with verbose output and log results
add_custom_target(run_tests
    COMMAND ctest --output-on-failure -V | tee /app/build/test_results.log
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running tests with verbose output and logging"
)


# Custom target to generate coverage report
add_custom_target(gen_cov
    COMMAND lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range > /dev/null 2>&1
    COMMAND lcov --rc lcov_branch_coverage=1 --remove coverage.info '*/googletest/*' '/usr/*' '*/test/*' --output-file coverageN.info --ignore-errors inconsistent --ignore-errors range > /dev/null 2>&1
    COMMAND genhtml --branch-coverage coverageN.info --output-directory coverage_report --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range > /dev/null 2>&1
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating coverage report"
)

# Enable coverage reporting option
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
        src/User.cpp
        src/Augment.cpp
        src/Employer.cpp
        src/Auth.cpp
        test/MatcherUnitTests.cpp
        test/DatabaseUnitTests.cpp
        test/ListingUnitTests.cpp
        test/testMain.cpp
        test/EmployerUnitTests.cpp
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

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
    external_libraries/openssl-3.3.2/include
    external_libraries/cpp_redis/includes
    /usr/local/include
    /opt/homebrew/Cellar/asio/1.30.2/include)

# Add coverage flags for GCC/Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

link_directories(external_libraries/WordNet-3.0/lib /usr/local/lib)

add_custom_target(unbind_port
    COMMAND lsof -t -i tcp:${USED_PORT} | xargs -r kill -9 || true
    COMMENT "Port ${USED_PORT} in use from earlier run.  Killing process before running."
)

# Main project executable
add_executable(LionHackerzProject 
    src/main.cpp 
    src/Database.cpp
    src/Dimension.cpp
    src/Matcher.cpp
    src/RouteController.cpp
    src/Listing.cpp
    src/User.cpp
    src/Augment.cpp
    src/Employer.cpp
    src/Auth.cpp
    src/Worker.cpp
)


# Include directories for main executable
target_include_directories(LionHackerzProject PUBLIC ${INCLUDE_PATHS})

# Find and link with libcurl
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(LionHackerzProject PRIVATE WN CURL::libcurl OpenSSL::SSL OpenSSL::Crypto cpp_redis tacopie nlohmann_json::nlohmann_json)

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


# Test executable
 add_executable(LionHackerzProjectTests
   test/testMain.cpp
   test/MatcherUnitTests.cpp
   test/DatabaseUnitTests.cpp
   test/ListingUnitTests.cpp
#    test/EmployerUnitTests.cpp
   test/DimensionUnitTests.cpp
   test/AugmentUnitTests.cpp
#    test/UserUnitTests.cpp
   test/AuthUnitTests.cpp
   test/AuthIntegrationTests.cpp
   test/DatabaseIntegrationTests.cpp
   test/MatcherUserDimensionIntegrationTests.cpp
   src/Matcher.cpp
   src/Database.cpp
   src/Listing.cpp
   src/User.cpp
   src/Augment.cpp
   src/Employer.cpp
   src/Auth.cpp
   src/Dimension.cpp
#    src/main.cpp
   src/RouteController.cpp
   src/Worker.cpp
 )

target_compile_options(LionHackerzProjectTests PRIVATE -O0 -fno-inline -fno-builtin -fno-omit-frame-pointer -fno-optimize-sibling-calls)

target_include_directories(LionHackerzProjectTests PRIVATE 
     ${INCLUDE_PATHS} 
     ${GTEST_INCLUDE_DIRS})

target_link_libraries(LionHackerzProjectTests PRIVATE 
     WN
     CURL::libcurl 
     OpenSSL::SSL 
     OpenSSL::Crypto
     cpp_redis
     tacopie
     GTest::gtest
     GTest::gtest_main
     pthread
     nlohmann_json::nlohmann_json)

add_custom_target(run_tests
    COMMAND ctest
    COMMENT "Running tests"
)

add_custom_target(gen_cov
    # COMMAND lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range > /dev/null 2>&1
    # COMMAND lcov --rc lcov_branch_coverage=1 --no-external --remove coverage.info '*/external_libraries/*' '*/opt/homebrew/*' '*/googletest/*' '/usr/*' '*/test/*' --output-file coverageN.info --ignore-errors inconsistent --ignore-errors range > /dev/null 2>&1
    # COMMAND genhtml --branch-coverage coverageN.info --output-directory coverage_report --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range > /dev/null 2>&1
    # commented out below to debug github workflow:
    # COMMAND python3 filterbr.py coverageN.info > coverageN_.info
    # COMMAND genhtml --branch-coverage coverageN_.info --output-directory coverage_report --ignore-errors inconsistent --ignore-errors corrupt --ignore-errors range > /dev/null 2>&1
    COMMAND mkdir -p coverage_report_gcovr
    COMMAND gcovr --root .. --object-directory . --html --html-details -o coverage_report_gcovr/coverage.html -f ../src -v --gcov-ignore-errors=no_working_dir_found --exclude-throw-branches --exclude-unreachable-branches --decisions
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating coverage report"
)

enable_testing()
gtest_discover_tests(LionHackerzProjectTests)
 

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
        src/Dimension.cpp
        src/Matcher.cpp
        src/RouteController.cpp
        src/Listing.cpp
        src/User.cpp
        src/Augment.cpp
        src/Employer.cpp
        src/Worker.cpp
        src/Auth.cpp
        test/MatcherUnitTests.cpp
        test/DatabaseUnitTests.cpp
        test/ListingUnitTests.cpp
        test/testMain.cpp
        test/EmployerUnitTests.cpp
        test/UserUnitTests.cpp
        test/DimensionUnitTests.cpp
        test/AugmentUnitTests.cpp
        test/AuthUnitTests.cpp
        test/AuthIntegrationTests.cpp
        test/DatabaseIntegrationTests.cpp
        test/MatcherUserDimensionIntegrationTests.cpp
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
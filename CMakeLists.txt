cmake_minimum_required(VERSION 3.20)
project(netsim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/storage_types.cpp
        src/reports.cpp
        src/simulation.cpp
        )

set(SOURCES_FILES_TESTS
#        test/test_nodes.cpp
#        test/test_package.cpp
#        test/test_storage_types.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        )

# Dodaj konfigurację typu `Test`.
add_executable(netsim ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp )

target_include_directories(netsim PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)


target_link_libraries(netsim gmock)
target_compile_definitions(netsim PUBLIC EXERCISE_ID=4)


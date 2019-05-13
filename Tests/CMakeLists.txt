cmake_minimum_required(VERSION 3.10)
project(Test)

set(CMAKE_C_STANDARD  11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fsanitize=address -fno-omit-frame-pointer")

enable_testing()
find_package(GTest)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    set(SOURCES ../rc_lib.c)
    file(GLOB_RECURSE TEST_SOURCES . TestCases/*.cpp)

    include_directories(${CMAKE_SOURCE_DIR})
    add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME} ${LIBS} gtest pthread)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
    )
else()
    message(WARNING "GTest not found, you won't be able to run the tests")
endif()
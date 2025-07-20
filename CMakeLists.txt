# Copyright 2025 Aleksa Radomirovic
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.25)
project(ulogc LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-Wall -Wextra")

option(BUILD_SHARED_LIBS "Build as shared library" ON)

add_library(
    ulogc
        src/logger.c
        src/logger.h
        src/lprintf.c
)
target_include_directories(
    ulogc
    PUBLIC SYSTEM
        include
)
target_compile_options(
    ulogc
    PRIVATE
        -fvisibility=hidden
)
set_target_properties(
    ulogc
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
)

link_libraries(ulogc)

find_package(GTest)

if(GTest_FOUND)
    enable_testing()
    enable_language(CXX)

    add_executable(LogTest EXCLUDE_FROM_ALL test/LogTest.cpp)
    target_link_libraries(LogTest PRIVATE gtest gtest_main)
    gtest_discover_tests(LogTest)
endif()

# Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

cmake_minimum_required(VERSION 3.16.3)

project(DDSZeroCopyExample VERSION 1 LANGUAGES CXX)

# Find requirements
if (NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif ()

if (NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif ()

#Check C++11
include(CheckCXXCompilerFlag)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if (NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif ()
endif ()

message(STATUS "Configuring ZeroCopyExample...")
file(GLOB DDS_ZERO_COPY_EXAMPLE_SOURCES_CXX "common/*.cxx")

add_executable(test_zombie_pub ${DDS_ZERO_COPY_EXAMPLE_SOURCES_CXX} zombie_pub.cpp)
target_compile_definitions(test_zombie_pub PRIVATE
        $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
        $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
        )
target_link_libraries(test_zombie_pub fastrtps fastcdr)

add_executable(test_zombie_sub ${DDS_ZERO_COPY_EXAMPLE_SOURCES_CXX} zombie_sub.cpp)
target_compile_definitions(test_zombie_sub PRIVATE
        $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
        $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
        )
target_link_libraries(test_zombie_sub fastrtps fastcdr)
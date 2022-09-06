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

add_executable(test_boost_reader reader.cc)
target_include_directories(test_boost_reader PRIVATE ${THIRDPARTY_BOOST_INCLUDE_DIR})
target_link_libraries(test_boost_reader)

add_executable(test_boost_writer writer.cc)
target_include_directories(test_boost_writer PRIVATE ${THIRDPARTY_BOOST_INCLUDE_DIR})
target_link_libraries(test_boost_writer ${THIRDPARTY_BOOST_LINK_LIBS})


add_executable(test_boost_reader_1.8 reader.cc)
target_include_directories(test_boost_reader_1.8 PRIVATE ../boost_1_80_0)
target_link_libraries(test_boost_reader_1.8)

add_executable(test_boost_writer_1.8 writer.cc)
target_include_directories(test_boost_writer_1.8 PRIVATE ../boost_1_80_0)
target_link_libraries(test_boost_writer_1.8)

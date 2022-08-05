cmake_minimum_required(VERSION 3.16.3)

project(MutexOwnerDeadTest VERSION 1 LANGUAGES CXX)

include_directories(${THIRDPARTY_BOOST_INCLUDE_DIR})

add_executable(_mutex_owner_dead_test mutex_owner_dead_test.cpp)

target_link_libraries(_mutex_owner_dead_test pthread)


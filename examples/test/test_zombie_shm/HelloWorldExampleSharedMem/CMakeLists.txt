cmake_minimum_required(VERSION 3.16.3)

project(DDSHelloWorldExampleDataSharing VERSION 1 LANGUAGES CXX)

file(GLOB DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX "*.cxx")

add_executable(_shared_mem_pub HelloWorldPublisher.cpp
        ${DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX})

add_executable(_shared_mem_sub HelloWorldSubscriber.cpp
        ${DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX})

target_link_libraries(_shared_mem_pub fastrtps fastcdr)
target_link_libraries(_shared_mem_sub fastrtps fastcdr)


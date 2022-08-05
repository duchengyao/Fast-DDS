cmake_minimum_required(VERSION 3.16.3)

project(DDSHelloWorldExampleDataSharing VERSION 1 LANGUAGES CXX)

file(GLOB DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX "*.cxx")

add_executable(_loans_pub LoanableHelloWorldPublisher.cpp
        ${DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX})

add_executable(_loans_sub LoanableHelloWorldSubscriber.cpp
        ${DDS_HELLOWORLD_EXAMPLE_DATASHARING_SOURCES_CXX})

target_link_libraries(_loans_pub fastrtps fastcdr)
target_link_libraries(_loans_sub fastrtps fastcdr)


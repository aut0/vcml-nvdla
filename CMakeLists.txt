 ##############################################################################
 #                                                                            #
 # Copyright 2020 Lukas Jünger                                                #
 #                                                                            #
 # Licensed under the Apache License, Version 2.0 (the "License");            #
 # you may not use this file except in compliance with the License.           #
 # You may obtain a copy of the License at                                    #
 #                                                                            #
 #     http://www.apache.org/licenses/LICENSE-2.0                             #
 #                                                                            #
 # Unless required by applicable law or agreed to in writing, software        #
 # distributed under the License is distributed on an "AS IS" BASIS,          #
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
 # See the License for the specific language governing permissions and        #
 # limitations under the License.                                             #
 #                                                                            #
 ##############################################################################

cmake_minimum_required(VERSION 3.5)
project(vcml-nvdla VERSION 0.1 LANGUAGES CXX)

option(VCML_NVDLA_BUILD_TESTS "Build unit tests" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(NVDLACmod REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

set(VCML_HOME $ENV{VCML_HOME})
if(NOT EXISTS ${VCML_HOME})
    set(VCML_HOME ${CMAKE_CURRENT_SOURCE_DIR}/deps/vcml)
endif()

if (NOT TARGET vcml)
    add_subdirectory(${VCML_HOME} vcml EXCLUDE_FROM_ALL)
endif()

set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(inc "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(sources
    "${src}/nvdla.cpp")

add_library(vcml-nvdla ${sources})
target_link_libraries(vcml-nvdla ${NVDLA_CMOD_LIBRARIES})
target_link_libraries(vcml-nvdla vcml)
target_link_libraries(vcml-nvdla ${SYSTEMC_LIBRARY})
target_include_directories(vcml-nvdla PUBLIC ${inc})
target_include_directories(vcml-nvdla SYSTEM PUBLIC ${NVDLA_CMOD_INCLUDE_DIR})

install(TARGETS vcml-nvdla DESTINATION lib)
install(FILES ${NVDLA_CMOD_LIBRARIES} DESTINATION lib)
install(DIRECTORY ${inc}/ DESTINATION include)

if(VCML_NVDLA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

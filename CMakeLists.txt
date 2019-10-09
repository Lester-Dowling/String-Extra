cmake_minimum_required (VERSION 3.15)
project (
  "String-Extra"
  VERSION 0.5.0
  DESCRIPTION "Common predicates and convenience functions for std::string."
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

#
# This project only has interface headers.  No translation units.
#
add_library( ${PROJECT_NAME} INTERFACE )

file(GLOB_RECURSE PUB_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME}
  INTERFACE  ${PUB_FILES}
  )
# Debug dump of public interface files:
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

# Top-level include directories:
target_include_directories(${PROJECT_NAME}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR})

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Parent directory for locally cached repositories of open source software (oss)
#
option(GRANT_INTERNET_ACCESS "Permit CMake to download from the Internet" OFF)
message(STATUS "Access Internet permission: ${GRANT_INTERNET_ACCESS}")
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  get_filename_component(HOME $ENV{USERPROFILE} ABSOLUTE)
else()
  get_filename_component(HOME $ENV{HOME} ABSOLUTE)
endif()
get_filename_component(LOCALLY_CACHED_REPOS_DIR "${HOME}/oss" ABSOLUTE)
if(EXISTS ${LOCALLY_CACHED_REPOS_DIR})
  message(STATUS "Locally cached repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
else()
  message(WARNING "No local repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
endif()

# Make globally available all required external libraries:
# add_subdirectory ("external")

#
# Boost library from external
#
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
  Boost::boost
  )

#
# Unit tests
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
message(STATUS "Building unit tests: ${BUILD_TESTING}")
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory("Unit-Tests")
endif()

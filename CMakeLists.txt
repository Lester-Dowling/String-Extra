cmake_minimum_required(VERSION 3.17)
project(
  "String-Extra"
  VERSION 0.5.0
  DESCRIPTION "Common predicates and convenience functions for std::string.")

#
# This project only has interface headers.  No translation units.
#
add_library(${PROJECT_NAME} INTERFACE)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Boost library from external
#
target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)

file(
  GLOB_RECURSE PUB_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME} INTERFACE ${PUB_FILES})
# Debug dump of public interface files:
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

#
# Unit tests
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Build unit tests only if this project isn't a submodule in a super-project.
    include(CTest)
    message(STATUS "Building unit tests: ${BUILD_TESTING}")
    if(BUILD_TESTING)
        add_subdirectory("Unit-Tests")
    endif(BUILD_TESTING)
endif()

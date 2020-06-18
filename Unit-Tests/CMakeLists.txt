cmake_minimum_required(VERSION 3.17)
set(TARGET "Unit-Tests")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(${TARGET} "GlobalFixture.hpp"
                         "GlobalFixture.cpp")
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
file(
  GLOB SRC_TEST_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test-*.cpp")
target_sources(${TARGET} PRIVATE ${SRC_TEST_FILES})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${TARGET} PRIVATE "pch-unit-tests.hpp")
# Debug dump of test src files:
foreach(src_file ${SRC_TEST_FILES})
  message(STATUS "${TARGET} src file: ${src_file}")
endforeach(src_file)


#
# Import the subjects for unit testing
#
target_link_libraries(${TARGET} PRIVATE "String-Extra")

if(WIN32)
  target_compile_definitions(
    ${TARGET}
    PRIVATE
    UNICODE
    _UNICODE
    _CONSOLE
    NOMINMAX
    WIN64
    _WIN64
    WINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP
    _WIN32_WINNT=_WIN32_WINNT_WIN10
    _CRT_SECURE_NO_WARNINGS
    _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  target_compile_options(${TARGET} PRIVATE -GR) # Turn on RTTI.
endif()


#
# Boost
#
if(NOT DEFINED CACHE{BOOST_REQUIRED_COMPONENTS})
    set(BOOST_REQUIRED_COMPONENTS "boost" CACHE INTERNAL "")
endif()

function(boost_require_component)
  foreach(boost_component ${ARGV})
    set(BOOST_REQUIRED_COMPONENTS "$CACHE{BOOST_REQUIRED_COMPONENTS};${boost_component}" CACHE INTERNAL "")
  endforeach(boost_component)
endfunction()

boost_require_component(filesystem date_time regex unit_test_framework)


add_subdirectory("boost")
target_link_libraries(${TARGET} PRIVATE Boost::boost)
foreach(Boost_Lib $CACHE{BOOST_REQUIRED_COMPONENTS})
  target_link_libraries(${TARGET} PRIVATE Boost::${Boost_Lib})
endforeach(Boost_Lib)
if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/RoboCopy-Log-Boost-redist-from-parent.txt"
        BOOST_REDIST_COPY_LOG)
  add_custom_command(
    TARGET ${TARGET} PRE_BUILD
    COMMAND
      START /MIN "RoboCopy of Boost redist from parent directory" #
      ROBOCOPY 
      /LOG:"${BOOST_REDIST_COPY_LOG}"
      /COPY:DAT /NP /R:2 /W:2 
      "${CMAKE_BINARY_DIR}"
      "${CMAKE_CURRENT_BINARY_DIR}" 
      "boost*dll")
endif(WIN32)

#
# Discover all the unit tests
#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(Discover.Boost.Test)
boost_discover_tests(${TARGET})

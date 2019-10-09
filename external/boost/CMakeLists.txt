cmake_minimum_required (VERSION 3.15)

set(BOOST_VERSION 1.71.0)

set(BUILD_SHARED_LIBS            ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_USE_DEBUG_LIBS        OFF)
set(Boost_USE_DEBUG_RUNTIME     OFF)
set(Boost_ADDITIONAL_VERSIONS   "${BOOST_VERSION}")
get_filename_component(BOOST_ROOT "$ENV{BOOST_ROOT}" ABSOLUTE)
if(NOT EXISTS ${BOOST_ROOT})
  message(FATAL_ERROR "BOOST_ROOT is not defined.")
endif()
get_filename_component(BOOST_LIBRARYDIR "$ENV{BOOST_LIBRARYDIR}" ABSOLUTE)
if(NOT EXISTS ${BOOST_LIBRARYDIR})
  # Try to guess location of Boost lib directory:
  if(WIN32)
    get_filename_component(BOOST_LIBRARYDIR "$ENV{BOOST_ROOT}/../../lib" ABSOLUTE)
  endif()
  if(NOT EXISTS ${BOOST_LIBRARYDIR})
    message(FATAL_ERROR "BOOST_LIBRARYDIR is not defined.")
  endif()
endif()
get_filename_component(BOOST_CMAKE_DIR "${BOOST_LIBRARYDIR}/cmake" ABSOLUTE)
if(EXISTS ${BOOST_CMAKE_DIR})
  find_package(
    Boost ${BOOST_VERSION} REQUIRED
    COMPONENTS ${Boost_LIBRARIES}
    PATHS ${BOOST_CMAKE_DIR}
    )
else()
  find_package(
    Boost ${BOOST_VERSION} REQUIRED
    COMPONENTS ${Boost_LIBRARIES}
    )
endif()


if (Boost_FOUND)
  set_target_properties(
	Boost::boost
	PROPERTIES IMPORTED_GLOBAL TRUE
	)
  target_compile_definitions(
	Boost::boost
	INTERFACE
	BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
	_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

  message(STATUS "------------------ CMake Variable Logging ------------------")
  message(STATUS "Boost_VERSION:               " ${Boost_VERSION})
  message(STATUS "BOOST_ROOT:                  " ${BOOST_ROOT})
  message(STATUS "BOOST_LIBRARYDIR:            " ${BOOST_LIBRARYDIR})
  message(STATUS "BOOST_CMAKE_DIR:             " ${BOOST_CMAKE_DIR})
  foreach(Boost_Lib ${Boost_LIBRARIES})
    message(STATUS "Boost_Lib:                   " ${Boost_Lib})
  endforeach(Boost_Lib)
  message(STATUS "---------------- End CMake Variable Logging ----------------")

endif(Boost_FOUND)

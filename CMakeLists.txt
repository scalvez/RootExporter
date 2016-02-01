# CMAKE_COMPILER
# - Basic CMake setup
# Check version meets ou requirements
# Declare project, which will configure compiler for us
cmake_minimum_required(VERSION 2.8)
project(FalaiseRootExporterPlugin)

# Use C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Ensure our code can see the Falaise headers
#include_directories(${Falaise_INCLUDE_DIRS})
include_directories(${Falaise_BUILDPRODUCT_DIR}/include)
include_directories(${Falaise_BUILDPRODUCT_DIR}/include/falaise)
include_directories(${Bayeux_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

###########################################################################################
# - GammaTracking modules:
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/falaise)

# - Headers:
list(APPEND FalaiseRootExporterPlugin_HEADERS
  source/falaise/snemo/exports/event_exporter.h
  # source/falaise/snemo/exports/export_ascii_event.h
  source/falaise/snemo/exports/export_event.h
  source/falaise/snemo/exports/export_root_event.h
  source/falaise/snemo/exports/loggable_support.h
  source/falaise/snemo/exports/root_utils.h
  source/falaise/snemo/processing/export_root_module.h
  # source/falaise/snemo/processing/export_ascii_module.h
  )

# - Sources:
list(APPEND FalaiseRootExporterPlugin_SOURCES
  source/falaise/snemo/exports/event_exporter.cc
  # source/falaise/snemo/exports/export_ascii_event.cc
  source/falaise/snemo/exports/export_event.cc
  source/falaise/snemo/exports/export_root_event.cc
  source/falaise/snemo/exports/loggable_support.cc
  source/falaise/snemo/exports/root_utils.cc
  source/falaise/snemo/processing/export_root_module.cc
  # source/falaise/snemo/processing/export_ascii_module.cc
  )

###########################################################################################

# Build a dynamic library from our sources
add_library(Falaise_RootExporter SHARED
  ${FalaiseRootExporterPlugin_HEADERS}
  ${FalaiseRootExporterPlugin_SOURCES})

target_link_libraries(Falaise_RootExporter Falaise)

# Apple linker requires dynamic lookup of symbols, so we
# add link flags on this platform
if(APPLE)
  set_target_properties(Falaise_RootExporter
    PROPERTIES LINK_FLAGS "-undefined dynamic_lookup"
    )
endif()

# Install it:
install(TARGETS Falaise_RootExporter DESTINATION ${CMAKE_INSTALL_LIBDIR}/Falaise/modules)

# - Publish headers
foreach(_hdrin ${FalaiseRootExporterPlugin_HEADERS})
  string(REGEX REPLACE "source/falaise/" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${Falaise_BUILDPRODUCT_DIR}/include/falaise/${_hdrout} @ONLY)
endforeach()

# Test support:
option(FalaiseRootExporterPlugin_ENABLE_TESTING "Build unit testing system for FalaiseRootExporter" ON)
if(FalaiseRootExporterPlugin_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(testing)

endif()

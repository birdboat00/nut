cmake_minimum_required(VERSION 3.20)
project(nut VERSION 0.1.0 LANGUAGES CXX)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(NUT_OPT_ENABLE_TESTS "Build nut examples" ${IS_TOPLEVEL_PROJECT})

if(NUT_OPT_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
  ARCH_INDEPENDENT)
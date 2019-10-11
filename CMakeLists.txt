cmake_minimum_required(VERSION 3.5)
project(waffle VERSION 0.1.0 LANGUAGES CXX)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set (CMAKE_CXX_STANDARD 17)

##############################################
# Create target and set properties
add_library(waffle
    src/waffle.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Waffle::waffle ALIAS waffle)

#Set target properties
target_include_directories(waffle
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(waffle PRIVATE cxx_auto_type)
target_compile_options(waffle PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

##############################################
# Installation instructions
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Waffle)

install(TARGETS waffle
    EXPORT waffle-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# This is required so that the exported target has the name Waffle and not waffle
set_target_properties(waffle PROPERTIES EXPORT_NAME Waffle)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT waffle-targets
  FILE
    WaffleTargets.cmake
  NAMESPACE
    Waffle::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/WaffleConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/WaffleConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/WaffleConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/WaffleConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/WaffleConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT waffle-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/WaffleTargets.cmake NAMESPACE Waffle::)

#Register package in user's package registry
export(PACKAGE Waffle)

add_subdirectory(test)

cmake_minimum_required(VERSION 3.8)
project(fastply VERSION 0.0.0)

# library definition
add_library(fastply INTERFACE)
target_compile_features(fastply INTERFACE cxx_std_14)
target_include_directories(fastply INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
add_library(fastply::fastply ALIAS fastply)

# installation info
install(TARGETS fastply EXPORT ${CMAKE_PROJECT_NAME}Targets
        INCLUDES DESTINATION include)
install(DIRECTORY include/fastply
        DESTINATION include
        COMPONENT Devel)

include(CMakePackageConfigHelpers)
set(ConfigPackageSource ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME})
set(ConfigPackageDestination lib/cmake/${CMAKE_PROJECT_NAME})
write_basic_package_version_file(
        ${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config-version.cmake
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(../cmake/config.cmake.in
        ${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${ConfigPackageDestination})

# install library
install(EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION ${ConfigPackageDestination}
        FILE ${CMAKE_PROJECT_NAME}-targets.cmake
        NAMESPACE fastply::
        COMPONENT Devel)
install(FILES
        "${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config.cmake"
        "${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config-version.cmake"
        DESTINATION ${ConfigPackageDestination}
        COMPONENT Devel)
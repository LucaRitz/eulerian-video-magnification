cmake_minimum_required(VERSION 3.15.1)
project(evm_main)

set(CMAKE_CXX_STANDARD 17)

OPTION(EVM_BUILD_TEST "Build tests" ON)
OPTION(EVM_BUILD_DOCUMENTATION "Build documentation" ON)
OPTION(EVM_BUILD_CODE_DOCUMENTATION "Build code documentation" ON)
OPTION(EVM_BUILD_EXT_LIBS "Build external libraries" ON)

if (EVM_BUILD_EXT_LIBS)
    message(STATUS "evm -- Build external dependencies")
    include(ext/externals.cmake)
else ()
    message(STATUS "evm -- External dependencies were not build")
endif ()

add_subdirectory(lib)

if (EVM_BUILD_TEST)
    message(STATUS "evm -- Build tests")
    add_subdirectory(test)
else()
    message(STATUS "evm -- Tests were not build")
endif()

if (EVM_BUILD_DOCUMENTATION)
    message(STATUS "evm -- Build documentation")
    add_subdirectory(doc_source)
else()
    message(STATUS "evm -- Documentation is not build")
endif()

if (EVM_BUILD_CODE_DOCUMENTATION)
    message(STATUS "evm -- Build code documentation")
    add_subdirectory(doxygen)
else()
    message(STATUS "evm -- Code documentation is not build")
endif()

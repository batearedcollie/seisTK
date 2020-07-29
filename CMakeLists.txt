# Copyright 2017 Bateared Collie
#
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this 
#   list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this 
#   list of conditions and the following disclaimer in the documentation and/or other
#   materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may 
#   be used to endorse or promote products derived from this software without specific
#   prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
# SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
# OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# set the required version
cmake_minimum_required(VERSION 3.8)

# Create a project
project(seisTK)


#######################################
# Add to the module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

#######################################
#
# These RPATH settings allow targets to run from the build tree
set(CMAKE_MACOSX_RPATH FALSE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)    
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

#######################################
# Add some general compile options - supress a few warnings
add_compile_options(-std=c++11)
if(APPLE)
    add_compile_options("-Wno-macro-redefined")
    add_compile_options("-Wno-#pragma-messages")
else()
endif()

#######################################
# Include Macros from Common
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Common)

#######################################
# OpenMP
set(ENABLE_OpenMP TRUE CACHE BOOL "Enable OpenMP" ) 
if(ENABLE_OpenMP)
    find_package(OpenMP REQUIRED)
    message(STATUS "OpenMP_C_FLAGS = ${OpenMP_C_FLAGS}")        
    message(STATUS "OpenMP_CXX_FLAGS = ${OpenMP_CXX_FLAGS}")    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    add_definitions(-D_OPENMP)    
endif()

#######################################
# Find Python

set(ENABLE_PYTHON TRUE CACHE BOOL "Enable Python Bindings" )

if(ENABLE_PYTHON) 
    find_package(PythonInterp REQUIRED)
    message("PYTHONINTERP_FOUND = ${PYTHONINTERP_FOUND}")
    message("PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
    message("PYTHON_VERSION_STRING = ${PYTHON_VERSION_STRING}")
endif(ENABLE_PYTHON)



#######################################
# VTK

find_package(VTK REQUIRED)
message(STATUS "VTK_PREFIX_PATH = ${VTK_PREFIX_PATH}")
message(STATUS "VTK_WRAP_PYTHON = ${VTK_WRAP_PYTHON}")
message(STATUS "VTK_LIBRARIES = ${VTK_LIBRARIES}")

#######################################
# Shared or static libs
set(_shared_default ON)
get_target_property(_vtk_libtype VTK::CommonCore TYPE)
if (_vtk_libtype STREQUAL "STATIC_LIBRARY")
  set(_shared_default OFF)
endif ()

option(BUILD_SHARED_LIBS "Build shared or static libraries" "${_shared_default}")
include(CTest)
include(GNUInstallDirs)
message(STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")

#######################################
# Scan the modules in the project

# Found that vtk_module_find_modules did not work 
#  so using a simple list

vtk_module_scan(
  MODULE_FILES      "${CMAKE_CURRENT_SOURCE_DIR}/DataTypes/vtk.module"
  REQUEST_MODULES   STK::DataTypes
  PROVIDES_MODULES  curr_modules
  ENABLE_TESTS      ON)
list(APPEND modules ${curr_modules})

vtk_module_scan(
  MODULE_FILES      "${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/vtk.module"
  REQUEST_MODULES   STK::Algorithms
  PROVIDES_MODULES  curr_modules
  ENABLE_TESTS      ON)
list(APPEND modules ${curr_modules})
message(STATUS "Building Modules = ${modules}")

#######################################
# Python Wrapping

vtk_module_python_default_destination(python_destination)

#######################################
# Module building

vtk_module_build(MODULES ${modules})

#######################################
# Python Wrapping

# Wrap it with Python.
vtk_module_wrap_python(
  MODULES         ${modules}
  PYTHON_PACKAGE  "stk"
  MODULE_DESTINATION "${python_destination}"
  # Static Python modules are almost never wanted.
  BUILD_STATIC    OFF
  INSTALL_HEADERS OFF)


#######################################
# Examples directory

add_subdirectory(Examples)

#######################################
# Custom Python modules

# Ability to disable Madagascar python extension
set(SEISTK_DISABLE_MADAGASCAR_EXT FALSE CACHE BOOL 
    "Disable the buildig of exnetsions for madagascar, use on systems where madagascar is not installed") 


#######################################
# Install the python module
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/setup.py" DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

# Handle cases where we are excluding Madagascar
set(PYTHON_BUILD_INSTALL_OPTIONS "")
if(SEISTK_DISABLE_MADAGASCAR_EXT)
    set(PYTHON_BUILD_INSTALL_OPTIONS "--without-madagascar-extension=True")
endif()

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp
  COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/setup.py" "build_py" "${PYTHON_BUILD_INSTALL_OPTIONS}" 
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(stk_build ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp)


# Install logic 
set(INSTALL_CODE "execute_process(")
set(INSTALL_CODE "${INSTALL_CODE} COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(INSTALL_CODE "${INSTALL_CODE} install --prefix=${CMAKE_INSTALL_PREFIX} ")
set(INSTALL_CODE "${INSTALL_CODE} --install-lib ${CMAKE_INSTALL_PREFIX}/lib/python3.6/site-packages/ ")
set(INSTALL_CODE "${INSTALL_CODE} ${PYTHON_BUILD_INSTALL_OPTIONS}")
set(INSTALL_CODE "${INSTALL_CODE} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}")
set(INSTALL_CODE "${INSTALL_CODE} )")
install(CODE "${INSTALL_CODE}")

#######################################
# Add the python dir 
add_subdirectory(stk)


#######################################
# Documentation
add_subdirectory(docs)



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
cmake_minimum_required(VERSION 3.2)

# Create a project
project(seisTK)

#######################################
# Add to the module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

#######################################

# These RPATH settings allow targets to run from the build tree
set(CMAKE_MACOSX_RPATH FALSE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)    
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")


#######################################
# Numpy
find_package(NumPy REQUIRED)
if(NOT NUMPY_FOUND)
    MESSAGE(FATAL_ERROR "Unable to find NumPy")
endif()
include_directories("${PYTHON_NUMPY_INCLUDE_DIR}")

#######################################
# VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
message(STATUS "VTK_LIBRARIES = ${VTK_LIBRARIES}")
message(STATUS "VTK_INSTALL_PREFIX = ${VTK_INSTALL_PREFIX}")

#######################################
# Python and vtk wrapping
find_package(PythonLibs 2.7 REQUIRED)
include(vtkWrapPython)
include_directories("${PYTHON_INCLUDE_PATH}")

#######################################
# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if(APPLE)
    # Updated to OSX High Sierra  and Home Brew install Boot python libs as 
    #  libboost_python27 etc. rather than libboost_python 
    #  CMake does not seem to like this but we can get it to work 
    #  if we pass in the component as python 27
    find_package(Boost REQUIRED COMPONENTS python27)
else()
    find_package(Boost REQUIRED COMPONENTS python)
endif()
if(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
endif()
include_directories("${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

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
# Add some general compile options - supress a few warnings
add_compile_options(-std=c++11)
if(APPLE)
    add_compile_options("-Wno-macro-redefined")
    add_compile_options("-Wno-#pragma-messages")
else()

endif()


#######################################
# Enable the tests
enable_testing()

#######################################
# Build the modules
add_subdirectory(cpp/DataTypes)
add_subdirectory(cpp/Algorithms)
add_subdirectory(cpp/Examples)

#add_subdirectory(stk)
#######################################
# Install the python module


file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/setup.py" DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/setup.py build 
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(stk_build ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp)


# Install logic 
set(INSTALL_CODE "execute_process(")
set(INSTALL_CODE "${INSTALL_CODE} COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(INSTALL_CODE "${INSTALL_CODE} install --prefix=${CMAKE_INSTALL_PREFIX}")
set(INSTALL_CODE "${INSTALL_CODE} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}")
set(INSTALL_CODE "${INSTALL_CODE} )")
install(CODE "${INSTALL_CODE}")


#######################################
# Add the python dir (for testing)
add_subdirectory(stk)

#######################################
# Documentation
add_subdirectory(docs)



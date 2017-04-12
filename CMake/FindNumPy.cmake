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


# Find the Python NumPy package
# PYTHON_NUMPY_INCLUDE_DIR
# PYTHON_NUMPY_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.6)

if(NOT PYTHON_EXECUTABLE)
  if(NumPy_FIND_QUIETLY)
    find_package(PythonInterp QUIET)
  else()
    find_package(PythonInterp)
    set(__numpy_out 1)
  endif()
endif()

if (PYTHON_EXECUTABLE)
  # Find out the include path
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
            "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
            OUTPUT_VARIABLE __numpy_path)
  # And the version
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
            "from __future__ import print_function\ntry: import numpy; print(numpy.__version__, end='')\nexcept:pass\n"
    OUTPUT_VARIABLE __numpy_version)
elseif(__numpy_out)
  message(STATUS "Python executable not found.")
endif(PYTHON_EXECUTABLE)

find_path(PYTHON_NUMPY_INCLUDE_DIR numpy/arrayobject.h
  HINTS "${__numpy_path}" "${PYTHON_INCLUDE_PATH}" NO_DEFAULT_PATH)

if(PYTHON_NUMPY_INCLUDE_DIR)
  set(PYTHON_NUMPY_FOUND 1 CACHE INTERNAL "Python numpy found")
endif(PYTHON_NUMPY_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy REQUIRED_VARS PYTHON_NUMPY_INCLUDE_DIR
                                        VERSION_VAR __numpy_version)
                            

####################################################################
# Old way which did not work so good...
#
#
# - Find the NumPy libraries
# This module finds if NumPy is installed, and sets the following variables
# indicating where it is.
#
# TODO: Update to provide the libraries and paths for linking npymath lib.
#
#  NUMPY_FOUND               - was NumPy found
#  NUMPY_VERSION             - the version of NumPy found as a string
#  NUMPY_VERSION_MAJOR       - the major version number of NumPy
#  NUMPY_VERSION_MINOR       - the minor version number of NumPy
#  NUMPY_VERSION_PATCH       - the patch version number of NumPy
#  NUMPY_VERSION_DECIMAL     - e.g. version 1.6.1 is 10601
#  NUMPY_INCLUDE_DIR         - path to the NumPy include files



#unset(NUMPY_VERSION)
#unset(NUMPY_INCLUDE_DIR)

#if(NOT PYTHON_EXECUTEABLE)
#  set(PYTHON_EXECUTABLE "${PYTHON_EXECUTABLE}")    
#endif()

#message(STATUS "found ${PYTHON_EXECUTABLE}")

#if(PYTHONINTERP_FOUND)
#if(PYTHON_EXECUTABLE)

#  execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c"
#    "import numpy as n; print(n.__version__); print(n.get_include());"
#    RESULT_VARIABLE __result
#    OUTPUT_VARIABLE __output
#    OUTPUT_STRIP_TRAILING_WHITESPACE)
#  if(__result MATCHES 0)
#    string(REGEX REPLACE ";" "\\\\;" __values ${__output})
#    string(REGEX REPLACE "\r?\n" ";"    __values ${__output})
#    #string(REGEX REPLACE "\r?\n" ";"    __values ${__values})      # Cause build error    
#    list(GET __values 0 NUMPY_VERSION)
#    list(GET __values 1 NUMPY_INCLUDE_DIR)
#    string(REGEX MATCH "^([0-9])+\\.([0-9])+\\.([0-9])+" __ver_check "${NUMPY_VERSION}")
#    if(NOT "${__ver_check}" STREQUAL "")
#      set(NUMPY_VERSION_MAJOR ${CMAKE_MATCH_1})
#      set(NUMPY_VERSION_MINOR ${CMAKE_MATCH_2})
#      set(NUMPY_VERSION_PATCH ${CMAKE_MATCH_3})
#      math(EXPR NUMPY_VERSION_DECIMAL
#        "(${NUMPY_VERSION_MAJOR} * 10000) + (${NUMPY_VERSION_MINOR} * 100) + ${NUMPY_VERSION_PATCH}")
#      string(REGEX REPLACE "\\\\" "/"  NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR})
#    else()
#     unset(NUMPY_VERSION)
#     unset(NUMPY_INCLUDE_DIR)
#     message(STATUS "Requested NumPy version and include path, but got instead:\n${__output}\n")
#    endif()
#  endif()
#else()
#  message(STATUS "To find NumPy need a python executable.")
#endif()

#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(NumPy REQUIRED_VARS NUMPY_INCLUDE_DIR NUMPY_VERSION
#                                        VERSION_VAR   NUMPY_VERSION)

#if(NUMPY_FOUND)
#  message(STATUS "NumPy ver. ${NUMPY_VERSION} found (include: ${NUMPY_INCLUDE_DIR})")
#endif()


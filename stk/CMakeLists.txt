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

# Installation directory
## todo make this a cache path but cannot get it working
#set(PYTHON_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages/stk")
set(PYTHON_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/stk")
#set(PYTHON_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/stk")

# Module list to work though - these will be built for python compatibility
set(PYTHON_MODULES "DataTypes" "Algorithms")

# Modules that will be linked but not built for python
#set(LINKED_MODULES "" )

# Listof VTK modules used
set(VTK_MODULES_USED vtkCommonDataModel vtkCommonCore vtkCommonExecutionModel) 
set(VTK_PYTHOND_LIBS)
foreach(TMP_LIB ${VTK_MODULES_USED})  
  set(pythonD ${TMP_LIB}PythonD)
  set(VTK_PYTHOND_LIBS ${VTK_PYTHOND_LIBS} ${pythonD})
endforeach()

# Add the includes
foreach(MOD ${PYTHON_MODULES})
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../cpp/${MOD}/")
endforeach()
foreach(MOD ${LINKED_MODULES})
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../cpp/${MOD}/")
endforeach()

#Add the non-built modules to link against
foreach(MOD ${LINKED_MODULES})
    list(APPEND PREVMODS Sp${MOD})
endforeach()

#Dummy target for all python builds
add_custom_target(pythonWrapping ALL)

# Make sure that -fPIC is included in the CXX_FLAGS and C_FLAGS
add_compile_options(-fPIC)
add_compile_options(-std=c++11)

# Work through modules and create python bindings
foreach(MOD ${PYTHON_MODULES})

    # Get a list of the source files for this module
    file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../cpp/${MOD}/*.cxx")

    # Wrapping    
    vtk_wrap_python3(${MOD} PY_SRC_FILES "${SRC_FILES}")

    set(PREVMODS_USE "${PREVMODS}")
    list(REVERSE PREVMODS_USE)
    
    # PythonD target
    add_library(${MOD}PythonD SHARED ${PY_SRC_FILES} ${SRC_FILES})
    target_link_libraries(${MOD}PythonD 
                            ${PREVMODS_USE}
                            ${VTK_PYTHOND_LIBS}
                            ${PYTHON_LIBRARIES}
                            vtkWrappingPythonCore
                            )
    
    # Shared library target
    add_library(${MOD} MODULE ${MOD}Init.cxx)
    target_link_libraries(${MOD} 
                        ${MOD}PythonD  
                        )

#                        ${PREVMODS_USE}
#                        ${VTK_PYTHOND_LIBS}
          
    set_target_properties(${MOD} PROPERTIES PREFIX "")
    if(APPLE) 
        set_target_properties(${MOD} PROPERTIES SUFFIX ".so")
    endif()
    if(WIN32 AND NOT CYGWIN)
        set_target_properties(${MOD} PROPERTIES SUFFIX ".pyd")
    endif(WIN32 AND NOT CYGWIN)

    # Add a dependecy to the main target
    add_dependencies(pythonWrapping ${MOD})

    # Install targets
    install(TARGETS ${MOD} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${PYTHON_INSTALL_DIR}
        ARCHIVE DESTINATION ${PYTHON_INSTALL_DIR}
        )

    install(TARGETS ${MOD}PythonD 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/
        )


    # Append to previous modules
    list(APPEND PREVMODS ${MOD}PythonD)
            
endforeach()


########################################################
# Copy python source tree into build directory (if required)
set(PYTHON_SRC_MODULES generators mdExt obspyExt)

if(NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  file(GLOB scripts "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
  foreach(script ${scripts})
     get_filename_component(name ${script} NAME)
     configure_file(${script} ${CMAKE_CURRENT_BINARY_DIR}/${name}  COPYONLY) 
  endforeach() 
  
  foreach(pymod ${PYTHON_SRC_MODULES})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${pymod})
    file(GLOB scripts "${CMAKE_CURRENT_SOURCE_DIR}/${pymod}/*.py")  
    foreach(script ${scripts})
        get_filename_component(name ${script} NAME)
        configure_file(${script} ${CMAKE_CURRENT_BINARY_DIR}/${pymod}/${name}  COPYONLY) 
    endforeach() 
  endforeach() 
endif()  


#####################################
# Add the examples directory (other directories are handled by python setup)

add_subdirectory(Examples)
    
    
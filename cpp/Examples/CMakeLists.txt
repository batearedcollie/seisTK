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

# Add Include paths
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../DataTypes")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../Algorithms")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../Common")


#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#######################################
# C executeables and test
add_executable(vtkHyperCubeTest vtkHyperCubeTest.cxx)
target_link_libraries(vtkHyperCubeTest stkDataTypes)

add_executable(vtkTraceHeaderTest vtkTraceHeaderTest.cxx)
target_link_libraries(vtkTraceHeaderTest stkDataTypes)

add_executable(vtkTracePanelTest vtkTracePanelTest.cxx)
target_link_libraries(vtkTracePanelTest stkDataTypes)

add_library(vtkSimpleTraceFilter SHARED vtkSimpleTraceFilter.cxx)
TARGET_LINK_LIBRARIES(vtkSimpleTraceFilter
                                stkDataTypes 
                                stkAlgorithms
                                ${Boost_LIBRARIES} 
                                ${PYTHON_LIBRARIES} 
                                ${VTK_LIBRARIES})

add_executable(vtkSimpleTraceFilterTest vtkSimpleTraceFilterTest.cxx )
target_link_libraries(vtkSimpleTraceFilterTest vtkSimpleTraceFilter)

#######################################
# Testing targets
#
# Note we explicitly set the LD_LIBARARY_PATH and DYLD_LIBRARY_PATH
#
set(Link_Libs "DataTypes" "Algorithms")
foreach(Lnk_LIB ${Link_Libs})
    set(LibDIR "${CMAKE_CURRENT_BINARY_DIR}/../${Lnk_LIB}")
    set(LibPathString "${LibPathString}:${LibDIR}")
endforeach()
set(LibPathString "${LibPathString}:${VTK_INSTALL_PREFIX}/lib")

set(LD_String "${LibPathString}:$ENV{LD_LIBRARY_PATH}")
set(DYLD_String "${LibPathString}:$ENV{DYLD_LIBRARY_PATH}")

# Now make testing and running targets

file(GLOB tests "${CMAKE_CURRENT_SOURCE_DIR}/*Test.cxx")
foreach(test ${tests})
    get_filename_component(name ${test} NAME_WE) 
        
    set(TEST_EXE "${CMAKE_CURRENT_BINARY_DIR}/${name}")
    message(STATUS "Adding test program ${TEST_EXE}") 
    
    add_custom_target("run_${name}"
                  COMMAND cmake "-E" "env" "DYLD_LIBRARY_PATH=:${DYLD_String}" "LD_LIBRARY_PATH=${LD_String}" "${TEST_EXE}"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  DEPENDS ${name}
                  VERBATIM
                  )   
    
    # Add to the test suite
    add_test(NAME ${name} 
           COMMAND cmake "-E" "env" "DYLD_LIBRARY_PATH=:${DYLD_String}" "LD_LIBRARY_PATH=${LD_String}" "${TEST_EXE}"
           WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
           )

endforeach()



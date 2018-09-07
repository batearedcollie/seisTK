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

#######################################
#Copy the test scripts overs

file(GLOB PYTHON_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )
file(COPY ${PYTHON_TESTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


#####################################
# Add test targets
#
# Note we explicitly set the LD_LIBARARY_PATH and DYLD_LIBRARY_PATH
#
#set(Link_Libs "DataTypes" "Algorithms")
#foreach(Lnk_LIB ${Link_Libs})
#    set(LibDIR "${CMAKE_CURRENT_BINARY_DIR}/../${Lnk_LIB}")
#    set(LibPathString "${LibPathString}:${LibDIR}")
#endforeach()

set(LibPathString "${CMAKE_CURRENT_BINARY_DIR}/../../stk/")
set(LibPathString "${LibPathString}:${CMAKE_CURRENT_BINARY_DIR}/../../cpp/DataTypes/")
set(LibPathString "${LibPathString}:${CMAKE_CURRENT_BINARY_DIR}/../../cpp/Algorithms/")
set(LibPathString "${LibPathString}:${VTK_INSTALL_PREFIX}/lib")

set(LD_String "${LibPathString}:${VTK_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH}")
set(DYLD_String "${LibPathString}:${VTK_INSTALL_PREFIX}/lib:$ENV{DYLD_LIBRARY_PATH}")

# Now make testing and running targets
file(GLOB tests "${CMAKE_CURRENT_BINARY_DIR}/*Test.py")
foreach(test ${tests})
    get_filename_component(name ${test} NAME_WE) 
    message(STATUS "Adding test program ${name}")   
    
    # Set up the command args
    if(SEISTK_DISABLE_MADAGASCAR_EXT)
        set(COMMAND_ARGS "--no-madagascar")
    endif() 

    add_custom_target("run_${name}"
                  COMMAND "${CMAKE_COMMAND}" 
                  "-E" "env" 
                  "DYLD_LIBRARY_PATH=:${DYLD_String}" 
                  "LD_LIBRARY_PATH=${LD_String}" 
                  "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}/../../:$ENV{PYTHONPATH}"
                  "${PYTHON_EXECUTABLE}" "./${name}.py" "${COMMAND_ARGS}"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  VERBATIM
                  )   
    
    # Add to the test suite
    add_test(NAME ${name} 
           COMMAND "${CMAKE_COMMAND}" 
           "-E" "env" 
           "DYLD_LIBRARY_PATH=:${DYLD_String}" 
           "LD_LIBRARY_PATH=${LD_String}" 
           "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}/../../:$ENV{PYTHONPATH}"
           "${PYTHON_EXECUTABLE}" "./${name}.py" "${COMMAND_ARGS}"
           WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
           )

endforeach()
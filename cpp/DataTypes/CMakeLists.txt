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

# Specify sources and library name
set(HDR_FILES vtkHyperCube.h
                vtkTracePanelData.h
                )
set(SRC_FILES vtkHyperCube.cxx
                vtkTracePanelData.cxx
                )
set(LIBNAME DataTypes)

#######################################
# Building the library
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# C++ Libraries are prefixed with Sp
set(LIBNAME "stk${LIBNAME}")

add_library(${LIBNAME} SHARED ${SRC_FILES})
TARGET_LINK_LIBRARIES(${LIBNAME} ${Boost_LIBRARIES} 
                                ${PYTHON_LIBRARIES} 
                                ${VTK_LIBRARIES})
if(APPLE) 
    set_target_properties(${LIBNAME} PROPERTIES SUFFIX ".so")
    set_target_properties(${LIBNAME} PROPERTIES PREFIX "lib")
endif()


#######################################
# Installation
install(TARGETS ${LIBNAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

foreach(hdr ${HDR_FILES})
    install(FILES "${hdr}" DESTINATION include)
endforeach()

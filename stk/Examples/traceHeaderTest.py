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
#

from stk.DataTypes import vtkHeaderTable
import stk.traceHeaderUtility as tu

import vtk

try:
    from obsln import UTCDateTime
except ImportError:
    from obspy import UTCDateTime

def BasicHeaderFunctionality():
    '''
    Tests and examples using basic header functionality
    '''
    
    
    # Make a header set
    header = vtkHeaderTable()
    
    # Add stations field
    v = vtk.vtkVariantArray()
    v.SetName("station")
    for i in range(0,10): v.InsertNextValue("stn-"+str(i))
    header.AddColumn(v)     
    
    # Add channels field
    v = vtk.vtkVariantArray()
    v.SetName("channel")
    for i in range(0,10): v.InsertNextValue("chn-"+str(i))
    header.AddColumn(v)   
    
    # Set a uniform value
    header.SetUniformValue("dt",vtk.vtkVariant(0.004))
    
    # Dump to screen
    print("\nBasic table")
    header.Dump()    
    
    # Set using a dictionary list aka. obspy
    header2 = vtkHeaderTable()
    tu.AddHeaderKeys(header2,{"station":"stn1","channel":"CHZ","nsmpl":100,
                              "starttime":UTCDateTime()})
   
    tu.AddHeaderKeys(header2,{"station":"stn2","channel":"CH1",
                              "nsmpl":100,
                              "endtime":UTCDateTime()})  
     
    print("\nDictionary Header assignment") 
    header2.Dump()
 
     
    # Retrieve as dictionary list (aka. obspy)
    rowDict=tu.GetTraceHeaderDict(1,header2)
    print("Retrieved row1: ",rowDict)

if __name__ == '__main__':
    BasicHeaderFunctionality()
    
    
    
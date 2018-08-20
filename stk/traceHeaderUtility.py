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

import vtk
from obspy import UTCDateTime 

def GetHeaderKeys(tHeader):
    '''
    Returns a list fo the header keys in traceheader object
    '''
    return [ tHeader.GetColumnName(i) for i in range(0,tHeader.GetNumberOfColumns())]

def ConvertKeys(inputDict,lambdaDict):
    '''
    Used to convert dictionary keys for representation in the trace header table and python

    @param inputDict: input dictionary to be converted
    @type inputDict: dictionary
    
    @param lambdaDict: dictionary containg lambda functions to apply to specifi keys
    @type lambdaDict: dictionary    
    
    @return: updated version of inputDict
    '''

    out ={}
    for kk in inputDict:
        if kk not in lambdaDict:
            out[kk]=inputDict[kk]
        else:
            out[kk]=lambdaDict[kk](inputDict[kk])
    
    return out
    
def ConvertKeysToVTK(inputDict,lambdaDict={"starttime": lambda x: x if x == None else UTCDateTime(x).isoformat(),
                                           "endtime": lambda x: x if x == None else UTCDateTime(x).isoformat(),
                                            
                                            },**kwargs):
    '''
    Used to convert dictionary keys for representation in the trace header table
    
    @param inputDict: input dictionary to be converted
    @type inputDict: dictionary
    
    @param lambdaDict: dictionary containing lambda functions to apply to specifi keys
    @type lambdaDict: dictionary    
    
    @return: updated version of inputDict
    '''
    return ConvertKeys(inputDict,lambdaDict)

def ConvertKeysFromVTK(inputDict,lambdaDict={"starttime": lambda x: x if x == None else  UTCDateTime(x),
                                             "endtime": lambda x: x if x== None else UTCDateTime(x),
                                            },**kwargs):
    '''
    Used to convert dictionary keys for representation in the trace header table
    
    @param inputDict: input dictionary to be converted
    @type inputDict: dictionary
    
    @param lambdaDict: dictionary containing lambda functions to apply to specifi keys
    @type lambdaDict: dictionary    
    
    @return: updated version of inputDict
    '''
    return ConvertKeys(inputDict,lambdaDict)

def AddHeaderKeys(tHeader,inputDict,**kwargs):
    '''
    Adds a row in the header table using inputDict 
    
    @param tHeader: traceHeader object to be updated
    @type tHeader: stk.DataTypes.vtkTraceHeader

    @param inputDict: input dictionary
    @type inputDict: dictionary
    '''

    dd = ConvertKeysToVTK(inputDict,**kwargs)

    # Get list of keys ion table
    cols = GetHeaderKeys(tHeader)
    
    # Add value for keys we already have in table
    addArray=vtk.vtkVariantArray()
    for cc in cols: 
        if cc in dd:
            # TODO - need lambda conversions here here
            addArray.InsertNextValue(dd[cc])
        else:
            addArray.InsertNextValue(None)
    insertID=0
    if addArray.GetNumberOfValues()!=0: insertID=tHeader.InsertNextRow(addArray)    

    # Add the columns that do not already exist
    for kk in inputDict:
        if kk not in cols:
            addArray=vtk.vtkVariantArray()
            for i in range(0,insertID+1): addArray.InsertNextValue(None)
            addArray.SetName(kk)
            tHeader.AddColumn(addArray)
            tHeader.SetValueByName(insertID,kk,dd[kk])

def GetTraceHeaderDict(traceID,tHeader,**kwargs):
    '''
    Returns a trace header as a dictionary
    '''

    hdr = tHeader.GetRow(traceID)
    
    cnames = GetHeaderKeys(tHeader)
    dd={}
    for cc in cnames: 
        vv = tHeader.GetValueByName(traceID,cc)
        type = vv.GetTypeAsString()
        dd[cc]=vtk.vtkVariantCast(vv,type)

    return ConvertKeysFromVTK(dd,**kwargs)
    

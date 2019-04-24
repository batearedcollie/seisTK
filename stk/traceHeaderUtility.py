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
    Returns a list for the header keys in traceheader object
    '''
    return [ tHeader.GetColumnName(i) for i in range(0,tHeader.GetNumberOfColumns())]

def AddBlankHeaderField(tHeader,fieldname):
    '''
    Adds a blank header field field to a header table
    '''
    nrow = tHeader.GetNumberOfRows()
    arr = vtk.vtkVariantArray()
    for i in range(0,nrow): arr.InsertNextValue(None)
    arr.SetName(fieldname)
    tHeader.AddColumn(arr)
    
def ConvertKeys(inputDict,lambdaDict):
    '''
    Used to convert dictionary keys for representation in the trace header table and python

    @param inputDict: input dictionary to be converted
    @type inputDict: dictionary
    
    @param lambdaDict: dictionary containg lambda functions to apply to specific keys
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
    
def ConvertKeysToVTK(inputDict,
                    lambdaDict={"starttime": lambda x: x if x == None else UTCDateTime(x).isoformat(),
                                "endtime": lambda x: x if x == None else UTCDateTime(x).isoformat(),
                                "processing": lambda x: str(x)
                                },
                    **kwargs):
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

def ExtractHeaderDataFromDict(inputDict,field_prefix=""):
    '''
    Extracts the field names and data for header fields from the keys of an input dictinoary
    If a dictionary item is itself a dictionary or dictionary like object the 
    the function is called recursively.
    
    Results is a single level dictionary where the key names are the fields for the header
    '''
    outDict={}
    for kk in inputDict:
        if isinstance(inputDict[kk],dict)==True or hasattr(inputDict[kk], '__dict__')==True:
                prefix = kk+"__"
                if field_prefix != "": prefix = field_prefix+prefix
                outDict.update( ExtractHeaderDataFromDict(inputDict[kk],field_prefix=prefix) )
        else:
            fieldname=kk
            if field_prefix != "": fieldname = field_prefix+kk
            outDict[fieldname]=inputDict[kk]
    return outDict

def AddHeaderKeys(tHeader,inputDict,create_new_row=True,**kwargs):
    '''
    Adds a row in the header table using inputDict. If an item in the input dictionary is 
    itself a dictionary or dictionary like object the function is called recursively 
    
    @param tHeader: traceHeader object to be updated
    @type tHeader: stk.DataTypes.vtkTraceHeader

    @param inputDict: input dictionary
    @type inputDict: dictionary
    
    @param field_prefix: string used to perform recusrive calls
    @type field_prefix: string
      
    '''

    dd = ConvertKeysToVTK(inputDict,**kwargs)

    # Extract field name and data from this input dictionary
    headerDict = ExtractHeaderDataFromDict(dd)
    #for kk in headerDict: print(kk)
    
    # Add the fields to the table
    existing_columns = GetHeaderKeys(tHeader)
    for ff in headerDict:
        if ff not in existing_columns: AddBlankHeaderField(tHeader,ff)
                
    # Create an array to insert as the next row
    fieldlist = GetHeaderKeys(tHeader)

    arr = vtk.vtkVariantArray()
    for ff in fieldlist: 
        if ff in headerDict: 
            arr.InsertNextValue( headerDict[ff] )
        else:
            arr.InsertNextValue( None )
    tHeader.InsertNextRow(arr) 
        
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
    

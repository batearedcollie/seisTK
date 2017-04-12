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
from vtk.util import numpy_support
from stk.DataTypes import vtkTracePanelData

from obspy.core.stream import Stream as obsStream
from obspy.core.trace import Trace as obsTrace

import numpy as np
import copy

def Generate(array=np.zeros([100,1]),
            traceDictList=[],
            delta=1.,
            origin=0.,
            name="TraceData",
            copy=True
            ):
    '''
    @summary: Generates a vtkTracePanelData object from an input numpy array
                
    @param array: input data array
    @type array: numpy nd array 
    @param traceDictList: trace dictionary
    @type traceDictList: [dict]  
    @param delta: time sampling   
    @type delta: float
    @param origin: origin time for traces
    @type origin: float
    @param name: Name for data array
    @type name: string
    @param copy: perform a copy of the source array (turn this off at own risk)
    @type copy: boolean      
    
    ** Example Usage **
    
    >>> from stk.obspyExt import tracePanel as tp

    >>> tp = tp.Generate(array=mat,
                       traceDict=dictionaryList,
                       delta=deltaT,
                       origin=0.,
                       )
                       
    .. warning::                    
 
         If you use copy=False then you should not try to access the 
            the trace data after processing.
    
    '''
    
    tpD = vtkTracePanelData()
    
    nsmpl=array.shape[-1]
    ntrace=1
    for i in range(0,len(array.shape)-1): ntrace=ntrace*array.shape[i]

    tpD.SetDimensions(nsmpl,ntrace,1)
    tpD.SetSpacing(delta,1.,1.)
    tpD.SetOrigin(origin,0.,0.)

    vtk_data = numpy_support.numpy_to_vtk(array.flatten(),
                                          deep=copy
                                          )
    vtk_data.SetName("TraceData")
    tpD.GetPointData().SetScalars(vtk_data)
    
    # Dictionary is copied by trace panel appendDict()
    for dd in traceDictList: tpD.appendDict(dd)
     
    return tpD

def ToTracePanelData(traces,origin_time=0.,name="TraceData"):
    '''
    @summary: Converts an obspy stream to a TracePanelData
    
    @param traces: input traces
    @type traces: obspy.core.stream
    @param origin_time: Origina time to apply to the traces
    @type origin_time: float
    @param name: name for the data set
    @type name: string
    
    ** Example Usage **
    
    >>> from stk.obspyExt import tracePanel as tp
    >>> tpD =tp.FromTraceData(dataset,name="TraceData")
    '''

    # Create the numpy object    
    tlen = traces[0].data.shape[0]
    for tr in traces:
        if tr.data.shape[0] > tlen: tlen = tr.data.shape[0] 
    mat = np.zeros([len(traces),tlen])
    for i,tr in  enumerate(traces): mat[i,:] = tr.data[:]
    
    # Create a list of trace dictionaries
    ll = []
    for tr in traces: ll.append(tr.stats.__dict__)
    
    return Generate(array=mat,
            traceDictList=ll,
            delta=traces[0].stats.delta,
            origin=origin_time,
            name=name,
            copy=True
            )

def ToObsPy(tracePanel,name="TraceData",useDictPointer=False):
    '''
    @summary: Generates a ObsPy stream object from a vtkTracePanelData object

    @param tracePanel: input vtkTracePanelData object
    @type tracePanel: vtkTracePanelData
    
    @param name: scalar array name to take from vtkTracePanelData object
    @type name: string
    
    @param useDictPointer: use a direct pointer to the C++ representation
                    of the trace header dictionaries. If you use this 
                    it will stop C++ being able to access the dictionary 
                    properly.
    @type useDictPointer: boolean
    
    ** Example Usage **
    
    >>> from stk.obspyExt import tracePanel as tp
    >>> td = tp.ToTraceData(tpD,name="TraceData")
    '''

    numComp = tracePanel.GetNumberOfScalarComponents()
    mat = numpy_support.vtk_to_numpy(tracePanel.GetPointData().GetScalars(name))
    dims = tracePanel.GetDimensions()
    out = obsStream([])

    if useDictPointer ==True:
        dList = tracePanel.GetDictionaryList()
    else:
        dList=[]
        tracePanel.GetDictionaryList(dList)

    print "dims =",dims[0],dims[1]

    for i in range(0,dims[1]):   
        
        print "Getting ",i
             
        dd=dList[i]
        
        for j in range(0,numComp):
            if numComp > 1:
                dd["vtkTracePanelComponent"] = j
                mm= mat[i*dims[0]:(i+1)*dims[0],j]                 
            else:
                mm= mat[i*dims[0]:(i+1)*dims[0]] 
            out.append(obsTrace(data=mm,header=dd))
      
    return out
                                     
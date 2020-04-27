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
from vtk.util import numpy_support
from stk.DataTypes import vtkTracePanelData

import stk.traceHeaderUtility as th

import stk 
if stk.useObsLn==True:
    from obsln.core import UTCDateTime
else:
    from obspy.core import UTCDateTime
    
import numpy as np

def tracePanelGenerate(array=np.zeros([1,100]),
            traceDictList=[{
                            'network' : 'Blah',
                            'station' : 'blah',
                            'channel' : 'blah',
                            'starttime' : UTCDateTime(),
                        }],
            delta=1.,
            origin=0.,
            name="TraceData",
            copy=True,
            **kwargs
            ):
    '''
    @summary: Generates a vtkTracePanelData object from an input numpy array
                
    @param array: input data array (assumed traces are row major)
    @type array: numpy nd array 
    @param traceDictList: trace dictionary list
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
    
    >>> import stk.generators as gen
    >>> tp = gen.tracePanelGenerate(array=mat,
                       traceDict=dictionaryList,
                       delta=deltaT,
                       origin=0.,
                       )
                       
    .. warning::                    
 
         If you use copy=False then you should not try to access the 
            the trace data after processing.
    
    '''
    

    # Number f samples
    nsmpl=array.shape[-1]
    ntrace=1
    for i in range(0,len(array.shape)-1): ntrace=ntrace*array.shape[i]    

    # sample rate
    smpl_rate=1./delta
    
    # Compute/check some dictionary stuff (if nec.)
    for dd in traceDictList:
        if "sampling_rate" not in dd: 
            dd["sampling_rate"]=smpl_rate
        else:
            if dd["sampling_rate"]!=smpl_rate: 
                raise Exception("Trace sample rate does not match the input value")
        
        if "delta" not in dd: 
            dd["delta"]=delta
        else:
            if dd["delta"]!=delta: 
                raise Exception("Trace sample delta does not match the input value")        
    
        if "npts" not in dd: 
            dd["npts"]=nsmpl
        else:
            if dd["npts"]!=nsmpl: 
                raise Exception("Trace nsmpl does not match the input data")      
    
    tpD = vtkTracePanelData()
    tpD.SetDimensions(nsmpl,ntrace,1)
    tpD.SetSpacing(delta,1.,1.)
    tpD.SetOrigin(origin,0.,0.)

    vtk_data = numpy_support.numpy_to_vtk(array.flatten(),
                                          deep=copy
                                          )
    vtk_data.SetName("TraceData")
    tpD.GetPointData().SetScalars(vtk_data)
    
    for dd in traceDictList: 
        th.AddHeaderKeys(tpD.GetTraceHeaderTable(),dd,**kwargs)


    return tpD
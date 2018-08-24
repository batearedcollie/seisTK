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
from obspy.core.stream import Stream
from obspy.core.trace import Trace as obsTrace
from obspy.core import UTCDateTime 
import numpy as np

import stk.tracePanelDataUtility as tu
import stk.traceHeaderUtility as th

import stk.obspyExt as stkObs
  

def TraceDataSet():
    '''
    Createing a simple HyperCube data object
    '''
 
    print "\n***********************************"
    print "Generic import"
    print "***********************************" 


    # Generate a simple trace panel data
    tdata = tu.tracePanelGenerate(
                        array=np.zeros([1,100]),
                        traceDictList=[{
                            'network' : 'Blah',
                            'station' : 'blah',
                            'channel' : 'blah',
                            'starttime' : UTCDateTime(),
                        }],
                        delta=1.,
                        origin=0.,
                        name="TraceData",
                          )
    print "Number of dimensions is ",tdata.GetNDimensions()
    print "Number of points = ",tdata.GetNumberOfPoints()

    print "Headers"
    tdata.GetTraceHeaderTable().Dump()

    dims=np.zeros([2],dtype=np.int)
         
    tdata.GetFullDimensions(dims)
    for i,nn in enumerate(dims):
        print "Length = ",nn," spacing =",tdata.GetAxisSpacing(i)," origin =",tdata.GetAxisOrigin(i)

def TraceDataObsPy():
    '''
    Creating a vtkTracePanelData using obsPy objects
    '''   

    print "\n***********************************"
    print "ObsPy functionality"
    print "***********************************" 
    
    # Make and obspy data stream
    dta = np.zeros(100)
    dta[50]=1.

    st = Stream()
    data = np.zeros([100])
    hdr={'sampling_rate' : 500, 
              'delta' : 1./500., 
              'npts' : 100,
              'network' : 'Blah',
              'station' : 'blah',
              'channel' : 'blah',
              'starttime' : UTCDateTime(),
        }
    tr = obsTrace(data=dta,header=hdr)
    st.append(tr)

    # Plot it 
    #st[0].plot()
 
    # Now make it into a trace panel data
    print "\nConverting to trace panel data"
    tracePanelData = stkObs.ToTracePanelData(st,origin_time=0.,name="TraceData")    
 
    dims = tracePanelData.GetDimensions()
    print "Working with trace panel data:"
    print "Dimensions = ",dims[0],dims[1],dims[2]
    print "Number of points = ",tracePanelData.GetNumberOfPoints()

    dlist=[]
    for i in range(0,dims[1]): dlist.append( th.GetTraceHeaderDict(i,tracePanelData.GetTraceHeaderTable()) )
    
    for i,dd in enumerate(dlist):
        print "Dictionary for trace ",i
        for kk in dd: print "\t",kk," : ",dd[kk]
       
#     # Convert back to obspy
    print "\nBack to ObsPy data"
    updatedObsPy = stkObs.ToObsPy(tracePanelData) 
    print updatedObsPy

if __name__ == '__main__':
    TraceDataSet()
    TraceDataObsPy()
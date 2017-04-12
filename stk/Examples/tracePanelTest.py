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

import stk.obspyExt.tracePanel as tp
  
def TraceDataSet():
    '''
    Creating a vtkTracePanelData using python convenience functions
    '''   

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
    
    print "\nMade some data in obspy"
    
    # Plot it 
    #st[0].plot()

    # Now make it into a trace panel data
    print "\nConverting to trace panel data"
    tracePanelData = tp.ToTracePanelData(st,origin_time=0.,name="TraceData")    

    dims = tracePanelData.GetDimensions()
    print "Working with trace panel data:"
    print "Dimensions = ",dims[0],dims[1],dims[2]
    print "Number of points = ",tracePanelData.GetNumberOfPoints()
        
    dList=[]
    tracePanelData.GetDictionaryList(dList) 
    for i,dd in enumerate(dList):
        print "Dictionary for trace ",i
        for kk in dd: print "\t",kk," : ",dd[kk]
        
    # Convert back to obspy
    print "\nBack to ObsPy data"
    updatedObsPy = tp.ToObsPy(tracePanelData) 
    print updatedObsPy

if __name__ == '__main__':
    TraceDataSet()
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
import stk.hyperCubeUtility as hu
import rsf.api as sfapi

import tempfile
import numpy as np
    
def ToHyperCubeData(MdgscrInput,name="HyperCubeData"):
    '''
    @summary: Converts an Madgascar input Object to vtkHyperCube
    
    @param MdgscrInput: input data cube 
    @type MdgscrInput: rsf.api.Input
    
    @param name: name for the data set
    @type name: string
    
    ** Example Usage **
    
    >>> import stk.m8rExt as md
    >>> cube=md.ToHyperCubeData(Input)
    '''
    
    # Work out number of dimensions
    delta=[]
    origin=[]
    axis=1

    nn = MdgscrInput.int("n"+str(axis))    
    while nn !=None and axis <= len(MdgscrInput.__array__().shape):
        
        delta.append( MdgscrInput.float("d"+str(axis)) )
        origin.append( MdgscrInput.float("o"+str(axis)) )        
        
        axis=axis+1
        nn = MdgscrInput.int("n"+str(axis))
            
    cube = hu.hyperCubeGenerate(array=MdgscrInput.__array__(),
                          delta=delta,
                          origin=origin,
                          name = name, 
                          )
    
    return cube

def M8rToHyperCubeData(m8rfile,name="HyperCubeData"):
    '''
    @summary: Converts an Madgascar m8r Object to vtkHyperCube
    
    @param m8rfile: input data cube (note this is destroyed)
    @type m8rfile: m8r.File
    
    @param name: name for the data set
    @type name: string
    
    ** Example Usage **
    
    >>> import stk.m8rExt as md
    >>> grid = sf.math(output="sin(x1)*cos(x2)",
                   n1=101,n2=101,
                   d1=0.2,d2=0.2,
                   o1=10,o2=-10.)[0]
    >>> cube=md.M8rToHyperCubeData(grid)        
    '''

    MdgscrInput=sfapi.Input(m8rfile.__str__())
    return ToHyperCubeData(MdgscrInput,name="HyperCubeData")

def ToMadagascar(hypercube,filename=None,
          name="HyperCubeData"):
    '''
    @summary: Generates a Madagascar Input Object from vtkHyperCube

    @param hypercube: input vtkHyperCube object
    @type hypercube: vtkHyperCube

    @param filename: filename for output (if not given a temp file is used)
    @type filename: string
    
    @param name: scalar array name to take from vtkTracePanelData object
    @type name: string
    
    ** Example Usage **
    
    >>> import stk.mdExt as md
    >>> mdObj = md.ToMadagascar(cube)
    '''
    
    if filename == None:
        ii, filename = tempfile.mkstemp(suffix=".rsf")
    
    file=sfapi.Output(filename)
    
    naxis = hypercube.GetNDimensions()
    dims=np.zeros([naxis],dtype=np.int)
    hypercube.GetFullDimensions(dims)
    for i,nn in enumerate(dims):
        file.put("n"+str(i+1),nn)
        file.put("d"+str(i+1),hypercube.GetAxisSpacing(i))
        file.put("o"+str(i+1),hypercube.GetAxisOrigin(i))

    data = numpy_support.vtk_to_numpy(hypercube.GetPointData().GetScalars(name))
    file.write(data)
    file.close()
    
    return file
    
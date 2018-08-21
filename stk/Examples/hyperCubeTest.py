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

import stk.hyperCubeUtility as hu
import stk.mdExt as md


import numpy as np
import m8r as sf

def HyperCubeDataSet():
    '''
    Creating a simple HyperCube data object
    '''

    print "\n***********************************"
    print "Generic import"
    print "***********************************" 
 
    # Generate a simple hyper cube
    cube = hu.hyperCubeGenerate(array=np.zeros([10,20,5,6],dtype=np.float),
                          delta=[0.1,0.2,1.,2.],
                          origin=[-0.5,0.,-10.,3.],
                          )
    print "Number of dimensions is ",cube.GetNDimensions()
    
    dims=np.zeros([4],dtype=np.int)
        
    cube.GetFullDimensions(dims)
    for i,nn in enumerate(dims):
        print "Length = ",nn," spacing =",cube.GetAxisSpacing(i)," origin =",cube.GetAxisOrigin(i)

def HyperCubeMadagascarTest():
    '''
    Converting to/from Madagascar/m8r
    '''
    
    print "\n***********************************"
    print "M8r import/export"
    print "***********************************" 
    
    # Make a madagascar file object
    grid = sf.math(output="sin(x1)*cos(x2)",
                   n1=101,n2=101,
                   d1=0.2,d2=0.2,
                   o1=10,o2=-10.)[0]
    
    #Grid informations
    print "Madagascar data cube"
    grid.sfin()
    
    print "\nConverting to vtkHyperCube"
    cube=md.M8rToHyperCubeData(grid)
    
    print "Number of dimensions is ",cube.GetNDimensions()
    
    dims=np.zeros([cube.GetNDimensions()],dtype=np.int)  
    cube.GetFullDimensions(dims)
    for i,nn in enumerate(dims):
        print "Length = ",nn," spacing =",cube.GetAxisSpacing(i)," origin =",cube.GetAxisOrigin(i)


    print "\nConverting back to madagascar"
    mdObj = md.ToMadagascar(cube)

    print "\nsfin:"
    mdObj.sfin()

    print "\nsfattr:"
    mdObj.attr()
        
if __name__ == '__main__':
    HyperCubeDataSet()
    HyperCubeMadagascarTest()
/*
Copyright 2017 Bateared Collie

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may
   be used to endorse or promote products derived from this software without specific
   prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**************************************/
// Includes
#include "vtkTracePanelData.h"
#include "vtkSimpleTraceFilter.h"
#include <iostream>

/**************************************/
// Definitions
using namespace std;




/**************************************/
// main
int main()
{
	Py_Initialize();
	{
		cout << "\n*********************************\n";


		// Make some trace data
		vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();
		int dims[2]={12,1};				// 1 trace of 12 samples
		trc->SetDimensions(dims);
		trc->AllocateScalars(VTK_FLOAT,1);
		double smpl[2]={0.004,1.};		// 4ms sample rate
		trc->SetSpacing(smpl);
		double org[2]={-1,0.};			// start of time axis at -1. seconds
		trc->SetOrigin(org);

		cout << "Input data =\n";
		for(int i=0;i<trc->GetFullDimensions()[0];i++){
			int coord[2]={i,0};
			float v = i*2.;
			trc->SetScalarComponentFromFloat(coord,0,v);
			cout << v  << ", ";
		}
		cout << endl;


		// Run the filter
		vtkSmartPointer<vtkSimpleTraceFilter> filter= vtkSmartPointer<vtkSimpleTraceFilter>::New();
		filter->SetInputData(trc);
		filter->Update();

		// Print the output
		vtkSmartPointer<vtkTracePanelData> out = filter->GetOutput();
		cout << "Output data =\n";
		for(int i=0;i<out->GetFullDimensions()[0];i++){
			int coord[2]={i,0};
			float v = out->GetScalarComponentAsFloat(coord,0);
			cout << v  << ", ";
		}
		cout << endl;


	}


	Py_Finalize();
	return 0;
}




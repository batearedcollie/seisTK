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
#include "vtkSmartPointer.h"

/**************************************/
// Definitions
using namespace std;

/**************************************/
// main
int main()
{

	// Need to have this so that the Python call back don't cause seg faults
	Py_Initialize();

	{
		cout << "\n\n*********************************\n";
		cout << "Basic test for vtkTracePanelData\n";

		vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();

		int dims[2]={125,10};			// 10 traces of 125 samples
		trc->SetDimensions(dims);
		trc->AllocateScalars(VTK_FLOAT,1);

		double smpl[2]={0.004,1.};		// 4ms sample rate
		trc->SetSpacing(smpl);

		double org[2]={-1,0.};			// start of time axis at -1. seconds
		trc->SetOrigin(org);

		// Create some trace headers
		for(int i=0;i<trc->GetFullDimensions()[1];i++){

			// Create a trace dictionary
			boost::python::dict dd;
			dd["sample_rate"]=0.004;
			dd["trace_id"]=i;
			dd["station_code"]="test";
			trc->appendDict((PyObject*)dd.ptr());
		}


		// Write out the trace data
		cout << "\nTraceData object:\n";
		trc->Print(cout);


	}
	Py_Finalize();

	//We use return =0 for tests because
	// Otherwise it breaks Makefiles
	return 0;
}





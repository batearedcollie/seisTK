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

#include "vtkVariantArray.h"

/**************************************/
// Definitions
using namespace std;

#if defined(_OPENMP)
	#include <omp.h>
#endif

/**************************************/
// main
int main()
{

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

		// Add some trace headers
		vtkSmartPointer<vtkHeaderTable> hdr= trc->GetTraceHeaderTable();

		vtkSmartPointer<vtkVariantArray> xpos = vtkSmartPointer<vtkVariantArray>::New();
		xpos->SetName("xpos");
		vtkSmartPointer<vtkVariantArray> ypos = vtkSmartPointer<vtkVariantArray>::New();
		ypos->SetName("ypos");
		vtkSmartPointer<vtkVariantArray> stn_id = vtkSmartPointer<vtkVariantArray>::New();
		stn_id->SetName("stn_id");
		for ( unsigned int i = 0; i < 10; i++ ) {
			xpos->InsertNextValue( vtkVariant( double(i*20.) ) );
			ypos->InsertNextValue( vtkVariant( double(i*-20.) ) );
			stn_id->InsertNextValue( vtkVariant(i) );
		}
		hdr->AddColumn(xpos);
		hdr->AddColumn(ypos);
		hdr->AddColumn(stn_id);


		// Add an axilary header
		vtkSmartPointer<vtkHeaderTable> ahdr = trc->AddBlankHeaderTable("picks");
		vtkSmartPointer<vtkVariantArray> traceID = vtkSmartPointer<vtkVariantArray>::New(); traceID->SetName("traceID");
		vtkSmartPointer<vtkVariantArray> time = vtkSmartPointer<vtkVariantArray>::New(); time->SetName("time");
		ahdr->AddColumn(traceID);
		ahdr->AddColumn(time);

		cout << "\nHeader table:\n";
		trc->GetTraceHeaderTable()->Dump();

		cout << "\nAux header: picks\n";
		trc->GetHeaderTable("picks")->Dump();

		// Write out the trace data
		cout << "\nTraceData object:\n";
		trc->Print(cout);

	}


	{

		cout << "\n\n*********************************\n";
		cout << "VTK TracePanel Data Threading Test\n";


		// Make trace data
		vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();
		int dims[2]={125,10};			// 10 traces of 125 samples
		trc->SetDimensions(dims);
		trc->AllocateScalars(VTK_FLOAT,1);
		double smpl[2]={0.004,1.};		// 4ms sample rate
		trc->SetSpacing(smpl);
		double org[2]={-1,0.};			// start of time axis at -1. seconds
		trc->SetOrigin(org);

		// Create some trace headers
		vtkSmartPointer<vtkVariantArray> c1 = vtkSmartPointer<vtkVariantArray>::New(); c1->SetName("station");
		vtkSmartPointer<vtkVariantArray> c2 = vtkSmartPointer<vtkVariantArray>::New(); c2->SetName("channel");
		vtkSmartPointer<vtkVariantArray> c3 = vtkSmartPointer<vtkVariantArray>::New(); c3->SetName("location");

		trc->GetTraceHeaderTable()->AddColumn(c1);
		trc->GetTraceHeaderTable()->AddColumn(c2);
		trc->GetTraceHeaderTable()->AddColumn(c3);

		for(int i=0;i<trc->GetFullDimensions()[1];i++){
			vtkSmartPointer<vtkVariantArray> row  = vtkSmartPointer<vtkVariantArray>::New();
			row->InsertNextValue( vtkVariant( std::string("stn")+std::to_string(i) ) );
			row->InsertNextValue( vtkVariant( "CHZ" ) );
			row->InsertNextValue( vtkVariant( i ) );
			trc->GetTraceHeaderTable()->InsertNextRow(row);
		}

		cout << "\nHeaders:\n";
		trc->GetTraceHeaderTable()->Dump();

		// Assign some values
		{
		int nPoint = trc->GetNumberOfPoints();
		float* ff = (float*) trc->GetScalarPointer();
		for(vtkIdType counter=0;counter<nPoint;counter++){
			*ff = 1.;
			ff++;
		}
		}


		// Start threading
		#if defined(_OPENMP)
		#pragma omp parallel
		{
		int tid = omp_get_thread_num();
		int ntd = omp_get_num_threads();
		#else
		int tid =0;
		int ntd = 1;
		#endif

		// Make local copy on this thread
		vtkSmartPointer<vtkTracePanelData> trc_lcl = vtkSmartPointer<vtkTracePanelData>::New();
		trc_lcl->DeepCopy(trc);

		// Do something
		{
		int nPoint = trc_lcl->GetNumberOfPoints();
		for(vtkIdType counter=0;counter<nPoint;counter++){
			int coord[4];
			int work[3];
			trc_lcl->GetNDPointFromId(counter,coord);
			float* ff = (float*) trc_lcl->GetScalarPointer(coord,work);
			*ff *= tid;
		}
		}

		float* ff = (float*) trc_lcl->GetScalarPointer();
		printf("Hello from thread %i of %i ff[10]=%f\n",tid,ntd,ff[10]);

		#if defined(_OPENMP)
		}
		#endif

	}

	{
		cout << "\n\n*********************************\n";
		cout << "Copying vtkTracePanelData\n";

		// Make some trace data
		vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();
		int dims[2]={125,10};			// 10 traces of 125 samples
		trc->SetDimensions(dims);
		trc->AllocateScalars(VTK_FLOAT,1);
		double smpl[2]={0.004,1.};
		trc->SetSpacing(smpl);
		double org[2]={-1,0.};			// start of time axis at -1. seconds
		trc->SetOrigin(org);

		// Add some trace headers
		{
			vtkSmartPointer<vtkHeaderTable> hdr = trc->AddBlankHeaderTable("traces",true);
			vtkSmartPointer<vtkVariantArray> xpos = vtkSmartPointer<vtkVariantArray>::New();
			xpos->SetName("xpos");
			vtkSmartPointer<vtkVariantArray> ypos = vtkSmartPointer<vtkVariantArray>::New();
			ypos->SetName("ypos");
			vtkSmartPointer<vtkVariantArray> stn_id = vtkSmartPointer<vtkVariantArray>::New();
			stn_id->SetName("stn_id");
			for ( unsigned int i = 0; i < 10; i++ ) {
				xpos->InsertNextValue( vtkVariant( double(i*20.) ) );
				ypos->InsertNextValue( vtkVariant( double(i*-20.) ) );
				stn_id->InsertNextValue( vtkVariant(i) );
			}
			hdr->AddColumn(xpos);
			hdr->AddColumn(ypos);
			hdr->AddColumn(stn_id);
		}
		{
			vtkSmartPointer<vtkHeaderTable> hdr = trc->AddBlankHeaderTable("table2",true);
			vtkSmartPointer<vtkVariantArray> xx = vtkSmartPointer<vtkVariantArray>::New();
			xx->SetName("xx");
			for ( unsigned int i = 0; i < 10; i++ ) {
				xx->InsertNextValue( vtkVariant(i) );
			}
			hdr->AddColumn(xx);
		}
		{
			cout << "Input header tables:\n";
			std::vector<std::string> hdrTableNames=  trc->GetHeaderTableNames();
			for(int i=0;i<hdrTableNames.size();i++){
				cout << "Table: " << hdrTableNames[i] << endl;
				trc->GetHeaderTable(hdrTableNames[i].c_str())->Dump();
			}
		}

		vtkSmartPointer<vtkTracePanelData> trc_copy = vtkSmartPointer<vtkTracePanelData>::New();
		trc_copy->DeepCopy(trc);
		{

			cout << "Copied header tables\n";
			std::vector<std::string> hdrTableNames=  trc_copy->GetHeaderTableNames();
			for(int i=0;i<hdrTableNames.size();i++){
				cout << "Table: " << hdrTableNames[i] << endl;
				trc->GetHeaderTable(hdrTableNames[i].c_str())->Dump();
			}
		}


	}


	//We use return =0 for tests because
	// Otherwise it breaks Makefiles
	return 0;
}





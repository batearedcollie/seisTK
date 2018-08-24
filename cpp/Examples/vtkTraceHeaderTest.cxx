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
#include "vtkHeaderTable.h"
#include "vtkSmartPointer.h"

#include "vtkVariant.h"
#include "vtkVariantArray.h"
#include "vtkDataSetAttributes.h"
#include "vtkFloatArray.h"

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
		cout << "Basic test for vtkTraceHeader\n";

		vtkSmartPointer<vtkHeaderTable> hdr = vtkSmartPointer<vtkHeaderTable>::New();

		// Set up basic table
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

		cout << "Basic table\n";
		hdr->Dump();

		// Simple enquiry
		std::vector<std::string> keyList = hdr->GetKeyList();
		cout << "\nHeader Keys:\n";
		for(int i=0;i<keyList.size();i++){
			cout << i << " : " << keyList[i] << endl;
		}

		cout << "\nResult of search for key 'xpos' " << hdr->KeyExists((const char*)"xpos") << endl;
		cout << "Result of search for key 'nsmpl' " << hdr->KeyExists((const char*)"nsmpl") << endl;


		// set a uniform value
		hdr->SetUniformValue("nsmpl",vtkVariant(100));
		cout << "\nUpdated table\n";
		hdr->Dump();


		// Set tuples
		vtkSmartPointer<vtkFloatArray> dir = vtkSmartPointer<vtkFloatArray>::New();
		dir->SetName("orientation");
		dir->SetNumberOfComponents(3);
		for(int i=0;i<hdr->GetNumberOfRows();i++){
			float tuple[3]={0.,0.,1.};
			dir->InsertNextTuple(tuple);
		}
		hdr->AddColumn(dir);
		cout << "\nUpdated table\n";
		hdr->Dump();

		// Updating tuples
		float vv[2]={-1.,1.};
		hdr->SetUniformTuple<float,vtkFloatArray>("key",2,vv);
		cout << "\nUpdated table\n";
		hdr->Dump();

		float tt[2]={-2.,-2.};
		hdr->SetUniformTuple<float,vtkFloatArray>("key",2,tt);
		cout << "\nUpdated table\n";
		hdr->Dump();

	}

	{
		cout << "\n\n*********************************\n";
		cout << "VTK TracHeader Threading Test\n";

		vtkSmartPointer<vtkHeaderTable> hdr = vtkSmartPointer<vtkHeaderTable>::New();

		// Set up basic table
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

		hdr->SetUniformValue("nsmpl",vtkVariant(100));
		cout << "\nUpdated table\n";
		hdr->Dump();

		#if defined(_OPENMP)
		#pragma omp parallel
		{
		int tid = omp_get_thread_num();
		int ntd = omp_get_num_threads();
		#else
		int tid =0;
		int ntd = 1;
		#endif

		vtkSmartPointer<vtkHeaderTable> hdr_lcl = vtkSmartPointer<vtkHeaderTable>::New();
		hdr_lcl->DeepCopy(hdr);
		hdr_lcl->SetValueByName(0,"nsmpl",vtkVariant( tid ));

		printf("tid=%i nsmpl-tid=%i\n",tid,hdr_lcl->GetValueByName(0,"nsmpl").ToInt());

		#if defined(_OPENMP)
		}
		#endif
	}

	return 0;
}



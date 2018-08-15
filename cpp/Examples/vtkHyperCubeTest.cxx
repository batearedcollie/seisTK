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
#include "vtkHyperCube.h"
#include "vtkSmartPointer.h"

#if defined(_OPENMP)
	#include <omp.h>
#endif

/**************************************/
// Definitions
using namespace std;

/**************************************/
// main
int main()
{


	{
		// 4D Example
		cout << "\n\n*********************************\n";
		cout << "4D Basic test for vtkHyperCube\n";

		vtkSmartPointer<vtkHyperCube> hCube = vtkSmartPointer<vtkHyperCube>::New();

		hCube->SetNDimensions(4);
		cout << "\nNumber of dimensions: " << hCube->GetNDimensions() << endl;

		int dims[4]={2,3,4,5};

		// set dimensions
		hCube->SetDimensions(dims);
		hCube->AllocateScalars(VTK_DOUBLE,1);

		cout << "Image dimensions: ";
		for(int i=0; i<3;i++){cout << hCube->GetDimensions()[i] << ", ";}
		cout << std::endl;

		cout << "Image full dimensions: ";
		for(int i=0; i<hCube->GetNDimensions();i++){cout << hCube->GetFullDimensions()[i] << ", ";}
		cout << std::endl;

		int* ext = hCube->GetExtent();
		cout << "Image Extents: ";
		for(int i=0; i<6;i++){cout << ext[i] << ", ";}
		cout << std::endl;

		int* fext = hCube->GetFullExtent();
		cout << "Image Full Extents: ";
		for(int i=0; i<2*hCube->GetNDimensions();i++){cout << fext[i] << ", ";}
		cout << std::endl;

		cout << "Number of points: " << hCube->GetNumberOfPoints() << endl;
		cout << "Number of cells: " << hCube->GetNumberOfCells() << endl;
		cout << "Number of full cells: " << hCube->GetFullNumberOfCells() << endl;
		cout << "Data Dimension: " << hCube->GetDataDimension() << endl;

		// Getting scalar pointer
		cout << "\nTest getting data:\n";
		double* origin= (double*) hCube->GetScalarPointer();
		//cout << "origin =" << origin << endl;
		{
		int coord[4]={0,0,0,0};
		double* t= (double*) hCube->GetScalarPointer(coord);
		cout << "0,0,0,0 test (should be zero) =" << t-origin << endl;
		}

		{
		int coord[4]={0,0,1,0};
		double* t= (double*) hCube->GetScalarPointer(coord);
		cout << "0,0,1,0 test (should be 6) =" << t-origin << endl;
		}

		{
		int coord[4]={0,0,0,1};
		double* t= (double*) hCube->GetScalarPointer(coord);
		cout << "0,0,0,1 test (should be 24) =" << t-origin << endl;
		}
		{
		int coord[4]={1,1,1,1};
		double* t= (double*) hCube->GetScalarPointer(coord);
		cout << "1,1,1,1 test (should be 33) =" << t-origin << endl;
		}

		// set spacings
		cout << "\nSetting spacings:\n";
		double spc[4]={1.,1.,1.,1.};
		hCube->SetSpacing(spc);

		double* ospc = hCube->GetSpacing();
		cout << "spacing="<< ospc[0] << ","<< ospc[1] << ","
						<< ospc[2] << "," << ospc[3] << endl;

		// Origins
		cout << "\nSetting origins:\n";
		double org[4]={1.,1.,1.,1};
		hCube->SetOrigin(org);
		double* oorg = hCube->GetOrigin();
		cout << "origin="<< oorg[0] << ","<< oorg[1] << ","
						<< oorg[2] << "," << oorg[3] << endl;

		// Compute structured coordinates
		cout << "\nCompute structured coords:\n";
		double coord[4]={1.5,2.5,3.5,4.5};
		int icoord[4];
		double pcoord[4];
		int ret= hCube->ComputeStructuredCoordinates(coord,icoord,pcoord);

		cout << "Point coordinates =" << coord[0] << ","  << coord[1] << ","
				 << coord[2] << ","  << coord[3] << endl;
		cout << "ret=" << ret << endl;
		cout << "icoord=" << icoord[0] << ","  << icoord[1] << ","
				 << icoord[2] << ","  << icoord[3] << endl;
		cout << "pcoord=" << pcoord[0] << ","  << pcoord[1] << ","
				 << pcoord[2] << ","  << pcoord[3] << endl;


		// Computing 3D from 4D coordinates and vice versa
		{
		cout << "\nComputing 3D from 4D coordinates and vice versa\n";

		int coord[4]={1,2,3,5};
		cout << "Input Coordindate "<< coord[0] << ","<< coord[1] << ","
						<< coord[2] << "," << coord[3] << endl;

		int* coord3D = hCube->Get3DcoordinateFromND(coord);
		int c3d[3];												// this stops a memory leak
		memcpy(c3d,coord3D,3*sizeof(int));
		cout << "3D Coordindate "<< c3d[0] << ","<< c3d[1] << "," << c3d[2] << endl;

		int* coordND = hCube->GetNDcoordinateFrom3D(c3d);
		cout << "ND Coordindate "<< coordND[0] << ","<< coordND[1] <<
				"," << coordND[2] << "," << coordND[3]  << endl;
		}


		// Compute structured coordinates from point ID
		{
		cout << "\nComputing Structured coordinates (3D space)\n";
		double pp[4];
		hCube->GetPoint(3,pp);
		cout << "Getting point 3 = "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3] << endl;
		hCube->GetPoint(9,pp);
		cout << "Getting point 9 =  "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3] << endl;
		hCube->GetPoint(100,pp);
		cout << "Getting point 100 =  "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3] << endl;
		}

		{
		cout << "\nComputing Coordinates in ND Space\n";
		int pp[4];
		vtkIdType idout;
		hCube->GetNDPointFromId(3,pp);
		cout << "Getting point 3 = "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3];
		idout=hCube->ComputePointId(pp);
		cout << " Backwards id=" << idout << endl;
		hCube->GetNDPointFromId(9,pp);
		cout << "Getting point 9 =  "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3];
		idout=hCube->ComputePointId(pp);
		cout << " Backwards id=" << idout << endl;

		hCube->GetNDPointFromId(100,pp);
		cout << "Getting point 100 =  "<< pp[0] << ","<< pp[1] << "," << pp[2] << "," << pp[3];
		idout=hCube->ComputePointId(pp);
		cout << " Backwards id=" << idout << endl;
		}


	}


	#if defined(_OPENMP)
	{
		cout << "\n***************************\nHyperCube threading test\n";

		// Make a 2D hyper cube
		vtkSmartPointer<vtkHyperCube> hCube = vtkSmartPointer<vtkHyperCube>::New();

		// Go through the points using threading
		hCube->SetNDimensions(2);
		cout << "\nNumber of dimensions: " << hCube->GetNDimensions() << endl;
		int dims[4]={1,3,2,2};

		// set dimensions
		hCube->SetDimensions(4,dims);
		hCube->AllocateScalars(VTK_FLOAT,1);

		cout << "Image dimensions: ";
		for(int i=0; i<4;i++){cout << hCube->GetFullDimensions()[i] << ", ";}
		cout << std::endl;

		int nPoint = hCube->GetNumberOfPoints();
		cout << "Number of points = " << nPoint << std::endl;

		#pragma omp parallel
		{
		int tid = omp_get_thread_num();
		printf("Hello from thread %i of %i\n",tid,omp_get_num_threads());

		float* ff0= (float*) hCube->GetScalarPointer();

		#pragma omp for
		for(vtkIdType counter=0;counter<nPoint;counter++){
			int coord[4];
			int work[3];
			hCube->GetNDPointFromId(counter,coord);
			float* ff = (float*) hCube->GetScalarPointer(coord,work);	// using thread safe version

			printf("counter=%lli thread=%i : Coordinate = %i,%i,%i,%i : Pointer %li\n",
					counter,tid,coord[0],coord[1],coord[2],coord[3],ff-ff0);


		}
		}


		// Make thread local copy
		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
			vtkSmartPointer<vtkHyperCube> hCube_lcl = vtkSmartPointer<vtkHyperCube>::New();

			hCube_lcl->DeepCopy(hCube);
			int nPoint = hCube_lcl->GetNumberOfPoints();
			for(vtkIdType counter=0;counter<nPoint;counter++){
				int coord[4];
				int work[3];
				hCube_lcl->GetNDPointFromId(counter,coord);
				float* ff = (float*) hCube_lcl->GetScalarPointer(coord,work);
				*ff *= tid;
			}
		}



	}
	#endif


	//We use return =0 for tests because
	// Otherwise it breaks Makefiles
	return 0;
}





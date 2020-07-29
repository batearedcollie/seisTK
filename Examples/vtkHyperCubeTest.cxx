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

		{
		int* hdims= hCube->GetFullDimensions();
		double* hspc= hCube->GetSpacing();
		double* horg = hCube->GetOrigin();

		cout << "\nGrid:\n";
		cout << "N: " << hdims[0] << ", " << hdims[1] << ", " << hdims[2] << ", " << hdims[3] << endl;
		cout << "D: " << hspc[0] << ", " << hspc[1] << ", " << hspc[2] << ", " << hspc[3] << endl;
		cout << "O: " << horg[0] << ", " << horg[1] << ", " << horg[2] << ", " << horg[3] << endl;
		}

		cout << "Point coordinates =" << coord[0] << ","  << coord[1] << ","
				 << coord[2] << ","  << coord[3] << endl;
		cout << "ret=" << ret << endl;
		cout << "icoord=" << icoord[0] << ","  << icoord[1] << ","
				 << icoord[2] << ","  << icoord[3] << endl;
		cout << "pcoord=" << pcoord[0] << ","  << pcoord[1] << ","
				 << pcoord[2] << ","  << pcoord[3] << endl;


		// Try a point outside the grid
		coord[0]=-1;
		cout << "Trying outside grid: " << coord[0] << ", " << coord[1] << ", " << coord[2] << ", " << coord[3] << endl;

		ret= hCube->ComputeStructuredCoordinates(coord,icoord,pcoord);
		cout << "ret= " << ret << endl;
		if(ret==1){
			cerr << "Test failure " << endl;
			return 1;
		}

		// Try an axis with a negative delta
		{
			vtkSmartPointer<vtkHyperCube> cc = vtkSmartPointer<vtkHyperCube>::New();
			cc->DeepCopy(hCube);
			double org[4]={0.,0.,0.,0};
			cc->SetOrigin(org);
			double spc[4]={1.,-1.,1.,1.};
			cc->SetSpacing(spc);
			int* hdims= cc->GetFullDimensions();
			double* hspc= cc->GetSpacing();
			double* horg = cc->GetOrigin();
			cout << "\nGrid:\n";
			cout << "N: " << hdims[0] << ", " << hdims[1] << ", " << hdims[2] << ", " << hdims[3] << endl;
			cout << "D: " << hspc[0] << ", " << hspc[1] << ", " << hspc[2] << ", " << hspc[3] << endl;
			cout << "O: " << horg[0] << ", " << horg[1] << ", " << horg[2] << ", " << horg[3] << endl;


			std::vector<double> pp={0.5,-1.1,0.5,0.5};

			cout << "pp=" << pp[0] << ", " << pp[1] << ", " << pp[2] << ", " << pp[3] << endl;

			ret= cc->ComputeStructuredCoordinates(pp.data(),icoord,pcoord);
			cout << "ret=" << ret << endl;
			if(ret!=1){
				cerr << "Test failure " << endl;
				return 1;
			}
			cout << "icoord=" << icoord[0] << ","  << icoord[1] << ","
					 << icoord[2] << ","  << icoord[3] << endl;
			cout << "pcoord=" << pcoord[0] << ","  << pcoord[1] << ","
					 << pcoord[2] << ","  << pcoord[3] << endl;

			// Try an out of bound on negative axis
			pp[1]=-2.1;
			cout << "\npp=" << pp[0] << ", " << pp[1] << ", " << pp[2] << ", " << pp[3] << endl;
			ret= cc->ComputeStructuredCoordinates(pp.data(),icoord,pcoord);
			cout << "ret=" << ret << endl;
			if(ret==1){
				cerr << "Test failure " << endl;
				return 1;
			}

		}

		// Another test
		{
			vtkSmartPointer<vtkHyperCube> cc = vtkSmartPointer<vtkHyperCube>::New();
			std::vector<int> nn={41,63,1};
			cc->SetDimensions(3,nn.data());
			std::vector<double> ss = {-25, 50, 1};
			cc->SetSpacing(ss.data());
			std::vector<double> oo = {-1500, -100, 0};
			cc->SetOrigin(oo.data());

			std::vector<double> pp={-1000, 1000, 0};
			cout << "\npp=" << pp[0] << ", " << pp[1] << ", " << pp[2] << endl;
			ret= cc->ComputeStructuredCoordinates(pp.data(),icoord,pcoord);

			cout << "ret=" << ret << endl;
			if(ret==1){
				cerr << "Test failure " << endl;
				return 1;
			}
		}


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

	{
		cout << "\n***************************\nHyperCube cube reduction test\n";

		vtkSmartPointer<vtkHyperCube> hCube = vtkSmartPointer<vtkHyperCube>::New();

		int dims[5]={3,1,4,1,5};
		hCube->SetDimensions(5,dims);
		hCube->AllocateScalars(VTK_FLOAT,1);

		cout << "Image dimensions: ";
		for(int i=0; i< hCube->GetNDimensions();i++){cout << hCube->GetFullDimensions()[i] << ", ";}
		cout << std::endl;

		int nPoint = hCube->GetNumberOfPoints();
		cout << "Number of points = " << nPoint << std::endl;
		int ijk[6];
		for(vtkIdType ii=0;ii<hCube->GetNumberOfPoints();ii++){
			hCube->GetNDPointFromId(ii,ijk);
			float* ff = (float*) hCube->GetScalarPointer(ijk);
			*ff=ii;
		}

		{
			int ijk[6];
			cout << "Original Data = ";
			for(vtkIdType ii=0;ii<hCube->GetNumberOfPoints();ii++){
				hCube->GetNDPointFromId(ii,ijk);
				cout << *(float*) hCube->GetScalarPointer(ijk) << ", ";
			}
			cout << endl;
		}

		hCube->RemoveNullDimensions();

		cout << "After removal of null dimensions \n";
		cout << "Number of dimensions = " << hCube->GetNDimensions() << endl;
		cout << "Image dimensions: ";
		for(int i=0; i<hCube->GetNDimensions();i++){cout << hCube->GetFullDimensions()[i] << ", ";}
		cout << std::endl;

		{
			int ijk[hCube->GetNDimensions()];
			cout << "Data: ";
			for(vtkIdType ii=0;ii<hCube->GetNumberOfPoints();ii++){
				hCube->GetNDPointFromId(ii,ijk);
				cout << *(float*) hCube->GetScalarPointer(ijk) << ", ";
			}
			cout << endl;
		}

		// List scalar names
		std::vector< std::string > ss = hCube->GetScalarNames();
		for(int ii=0;ii<ss.size();ii++){
			cout << "ArrayName = " << ss[ii] << endl;
		}

	}


	//We use return =0 for tests because
	// Otherwise it breaks Makefiles
	return 0;
}





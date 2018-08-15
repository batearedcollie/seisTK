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
#include "vtkMath.h"
#include "vtkObjectFactory.h"

/**************************************/
// Definitions

using namespace std;


/**************************************/
// Public methods

// New method
vtkStandardNewMacro(vtkHyperCube)

// Print self
void vtkHyperCube::PrintSelf(ostream& os, vtkIndent indent)
{
	os << indent << "vtkHyperCube:" << endl;
	os << indent << "Number of Dimensions: " <<this->GetNDimensions() << endl;
	int* dims= this->GetFullDimensions();
	int* ext = this->GetFullExtent();
	double* spc= this->GetSpacing();
	double* org= this->GetOrigin();
	for(int i=0;i<this->GetNDimensions();i++){
		os << indent << "Axis: " << i << endl;
		os << indent << indent << "Number of points: " << dims[i] << endl;
		os << indent << indent << "Extent: " << ext[2*i] << " , " <<  ext[2*i+1] << endl;
		os << indent << indent << "Spacing: " << spc[i] << endl;
		os << indent << indent << "Origin: " << org[i] << endl;
	}
	os << indent << "End vtkHyperCube" << endl;


	this->Superclass::PrintSelf(os,indent.GetNextIndent());
}

// Get Data
vtkHyperCube* vtkHyperCube::GetData(vtkInformation* info)
{
  return info? vtkHyperCube::SafeDownCast(info->Get(DATA_OBJECT())) : 0;
}

// Get Data
vtkHyperCube* vtkHyperCube::GetData(vtkInformationVector* v, int i)
{
	return vtkHyperCube::GetData(v->GetInformationObject(i));
}

// Shallow copy
void vtkHyperCube::ShallowCopy(vtkDataObject* src)
{

	this->Superclass::ShallowCopy(src);

	if (vtkHyperCube* const pdo = vtkHyperCube::SafeDownCast(src))
	{
		this->SetNDimensions(pdo->GetNDimensions());
		this->SetSpacing(pdo->GetSpacing());
		this->SetOrigin(pdo->GetOrigin());
		this->SetDimensions(pdo->GetFullDimensions());
	}
	this->Modified();
}

// Deep copy
void vtkHyperCube::DeepCopy(vtkDataObject* src)
{
	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif

	this->Superclass::DeepCopy(src);

	#if defined(_OPENMP)
	}
	#endif

	if (vtkHyperCube* const pdo = vtkHyperCube::SafeDownCast(src))
	{
		this->SetNDimensions(pdo->GetNDimensions());
		this->SetDimensions(pdo->GetFullDimensions());
		this->SetSpacing(pdo->GetSpacing());
		this->SetOrigin(pdo->GetOrigin());
	}
	this->Modified();

}

// Empty copy
void vtkHyperCube::EmptyCopy(vtkDataObject* src)
{
  if (vtkHyperCube* const pdo = vtkHyperCube::SafeDownCast(src))
    {
	  this->SetNDimensions(pdo->GetNDimensions());
	  this->SetDimensions(pdo->GetDimensions());
	  this->AllocateScalars(pdo->GetScalarType(),pdo->GetNumberOfScalarComponents());
	  this->SetSpacing(pdo->GetSpacing());
	  this->SetOrigin(pdo->GetOrigin());
	  this->GetPointData()->GetScalars()->SetName(pdo->GetPointData()->GetScalars()->GetName());
	  this->Modified();
    }
}

// Empty copy
void vtkHyperCube::UnAllocatedCopy(vtkDataObject* src)
{
  if (vtkHyperCube* const pdo = vtkHyperCube::SafeDownCast(src))
    {
	  this->SetNDimensions(pdo->GetNDimensions());
	  this->SetDimensions(pdo->GetDimensions());
	  this->SetSpacing(pdo->GetSpacing());
	  this->SetOrigin(pdo->GetOrigin());
	  this->Modified();
    }
}

//! Zero elements in double or float cube
void vtkHyperCube::Zero()
{
	 int *dims=this->GetDimensions();
	 for (int z = 0; z < dims[2]; z++){
		 for (int y = 0; y < dims[1]; y++){
			 for (int x = 0; x < dims[0]; x++){
				 USEHYPERCUBE_FLOATDOUBLE_POINTERMACRO(this,x,y,z,pp,
						 for(int j=0;j<this->GetNumberOfScalarComponents();j++){
							 *pp=0;
							 pp++;
						 }
				 )
			 }
		 }
	 }
	 this->Modified();
}

void  vtkHyperCube::Get3DcoordinateFromND(int* coordND, int coord3D[3])
{
	this->get3DcoordinateFromND(coordND,coord3D);
}

int*  vtkHyperCube::Get3DcoordinateFromND(int* coordND)
{
	if(this->iPoint.size()<3){
		this->iPoint.resize(3,0);
	}
	this->get3DcoordinateFromND(coordND,this->iPoint.data());
	return this->iPoint.data();
}

void  vtkHyperCube::GetNDcoordinateFrom3D(int coord3D[3],int* iPoint)
{
	this->getNDcoordinateFrom3D(coord3D,iPoint);
}

int*  vtkHyperCube::GetNDcoordinateFrom3D(int* coord3D)
{
	if(this->iPoint.size()<this->GetNDimensions()){
		this->iPoint.resize(this->GetNDimensions(),0);
	}
	this->getNDcoordinateFrom3D(coord3D,this->iPoint.data());
	return this->iPoint.data();
}

/*************************************/
/* Overriding vtkImageData functions */
/*************************************/

void vtkHyperCube::SetNDimensions(int N)
{
	this->NDimensions=N;
	if(N>3){
		this->FullExtent.resize(2*N,0);
		this->FullDimensions.resize(N,1);
		this->Spacing.resize(N,1.);
		this->Origin.resize(N,0.);
	}
	this->Modified();
}

void vtkHyperCube::SetDimensions(int i, int j, int k)
{
	int dims[3]={i,j,k};
	this->SetDimensions(dims);
}

void vtkHyperCube::SetDimensions(int* dims)
{

	// Compute extents vector
	std::vector<int> ext = this->makeExtentsFromDims(dims);

	// Set the Full extents
	this->SetFullExtent(ext.data());
}

void  vtkHyperCube::SetDimensions(int Ndim, int* dims)
{
	this->SetNDimensions(Ndim);
	this->SetDimensions(dims);
}

int* vtkHyperCube::GetFullDimensions(){

	this->GetFullDimensions(this->FullDimensions.data());
	return this->FullDimensions.data();
}

void vtkHyperCube::GetFullDimensions (int* dOut)
{
	  for(int i=0;i<this->NDimensions;i++){
		  dOut[i] = this->FullExtent[2*i+1] - this->FullExtent[2*i] + 1;
	  }
}

void vtkHyperCube::SetFullExtent(int* ext)
{
	// Set full extents
	for(int i=0;i < 2*this->NDimensions;i++){this->FullExtent[i]=ext[i];}

	// Set 3D extents
	if(this->NDimensions==3){
		this->SetExtent(ext);
	}else{
		int ext3d[6]={0,0,0,0,0,0};
		ext3d[0]=ext[0]; ext3d[1]=ext[1];
		if(this->NDimensions>1){ext3d[2]=ext[2]; ext3d[3]=ext[3]; }
		if(this->NDimensions>2){ext3d[4]=ext[4]; ext3d[5]=ext[5]; }
		if(this->NDimensions>3){
			int axislen=ext3d[5]-ext[4]+1;
			for(int i=3;i<this->NDimensions;i++){
				int e1=ext[2*i];
				int e2=ext[2*i+1];
				axislen=axislen*(e2-e1+1);
			}
			ext3d[4]=0;
			ext3d[5]=axislen-1;
		}
		this->SetExtent(ext3d,false);
	}
	this->Modified();
}

int vtkHyperCube::GetDataDimension()
{
	if(this->NDimensions<=3) return vtkImageData::GetDataDimension();

	int m=0;
	for(int i=0;i<this->NDimensions;i++){
		if(this->FullExtent[2*i+1]-this->FullExtent[2*i]>0){
			m=m+1;
		}
	}
	return m;
}

vtkIdType vtkHyperCube::GetFullNumberOfCells()
{
  vtkIdType nCells=1;
  int i;

  int* dims = this->GetFullDimensions();

  for (i=0; i<this->NDimensions; i++)
    {
    if (dims[i] == 0)
      {
      return 0;
      }
    if (dims[i] > 1)
      {
      nCells *= (dims[i]-1);
      }
    }

  return nCells;
}

int vtkHyperCube::ComputeStructuredCoordinates (const double* x,
											int* ijk, double* pcoords)
{
	return this->ComputeStructuredCoordinates(x,ijk,pcoords,
												this->FullExtent.data(),
												this->Spacing.data(),
												this->Origin.data(),
												this->GetBounds());
}

int vtkHyperCube::ComputeStructuredCoordinates( const double* x,
											int* ijk,
											double* pcoords,
											const int* extent,
											const double* spacing,
											const double* origin,
											const double* bounds)
{
	// tolerance is needed for 2D data (this is squared tolerance)
	const double tol2 = 1e-12;

	//
	//  Compute the ijk location
	//
	int isInBounds = 1;
	for (int i = 0; i < this->NDimensions; i++)
	{
		double d = x[i] - origin[i];
	    double doubleLoc = d / spacing[i];

	    // Floor for negative indexes.
	    ijk[i] = vtkMath::Floor(doubleLoc);

	    pcoords[i] = doubleLoc - static_cast<double>(ijk[i]);

	    int tmpInBounds = 0;
	    int minExt = extent[i*2];
	    int maxExt = extent[i*2 + 1];

	    // check if data is one pixel thick
	    if ( minExt == maxExt )
	      {
	      double dist = x[i] - bounds[2*i];
	      if (dist*dist <= spacing[i]*spacing[i]*tol2)
	        {
	        pcoords[i] = 0.0;
	        ijk[i] = minExt;
	        tmpInBounds = 1;
	        }
	      }

	    // low boundary check
	    else if ( ijk[i] < minExt)
	      {
	      if ( (spacing[i] >= 0 && x[i] >= bounds[i*2]) ||
	           (spacing[i] < 0 && x[i] <= bounds[i*2 + 1]) )
	        {
	        pcoords[i] = 0.0;
	        ijk[i] = minExt;
	        tmpInBounds = 1;
	        }
	      }

	    // high boundary check
	    else if ( ijk[i] >= maxExt )
	      {
	      if ( (spacing[i] >= 0 && x[i] <= bounds[i*2 + 1]) ||
	           (spacing[i] < 0 && x[i] >= bounds[i*2]) )
	        {
	        // make sure index is within the allowed cell index range
	        pcoords[i] = 1.0;
	        ijk[i] = maxExt - 1;
	        tmpInBounds = 1;
	        }
	      }

	    // else index is definitely within bounds
	    else
	      {
	      tmpInBounds = 1;
	      }

	    // clear isInBounds if out of bounds for this dimension
	    isInBounds = (isInBounds & tmpInBounds);
	    }

	  return isInBounds;
}

vtkIdType vtkHyperCube::ComputePointId (int* ijk, int coord_workspace[3])
{
	this->get3DcoordinateFromND(ijk,coord_workspace);
	return vtkImageData::ComputePointId(coord_workspace);
}

vtkIdType vtkHyperCube::ComputePointId (int* ijk)
{
	this->get3DcoordinateFromND(ijk,this->coord);
	return vtkImageData::ComputePointId(this->coord);
}

vtkIdType vtkHyperCube::ComputeCellId(int* ijk,int coord_workspace[3])
{
	this->get3DcoordinateFromND(ijk,coord_workspace);
	return vtkImageData::ComputeCellId(coord_workspace);
}

vtkIdType vtkHyperCube::ComputeCellId(int* ijk)
{
	this->get3DcoordinateFromND(ijk,this->coord);
	return vtkImageData::ComputeCellId(this->coord);
}

void vtkHyperCube::SetAxisUpdateExtent(int 	idx,
									int 	min,
									int 	max,
									const int * 	updateExtent,
									int * 	axisUpdateExtent
									)
{
	  if (idx > this->NDimensions-1){
	    vtkWarningMacro("illegal axis!");
	    return;
	  }

	  memcpy((void*)axisUpdateExtent, (void*)updateExtent, this->NDimensions*2*sizeof(int));
	  if (axisUpdateExtent[idx*2] != min){axisUpdateExtent[idx*2] = min;}

	  if (axisUpdateExtent[idx*2+1] != max){axisUpdateExtent[idx*2+1] = max;}
}

void vtkHyperCube::GetAxisUpdateExtent(int idx, int &min, int &max,
                                       const int* updateExtent)
{
	if (idx > this->NDimensions-1){
		vtkWarningMacro("illegal axis!");
		return;
	}
	min = updateExtent[idx*2];
	max = updateExtent[idx*2+1];
}

void* vtkHyperCube::GetScalarPointerForExtent(int* extent, int* coord_workspace,
												int coord3d_workspace[3])
{
	for(int i=0;i<this->NDimensions;i++){
		coord_workspace[i]=extent[2*i];
	}
	return this->GetScalarPointer(coord_workspace,coord3d_workspace);
}

void* vtkHyperCube::GetScalarPointerForExtent(int* extent)
{
	for(int i=0;i<this->NDimensions;i++){
		this->iPoint[i]=extent[2*i];
	}
	return this->GetScalarPointer(this->iPoint.data());
}

void* vtkHyperCube::GetScalarPointer(int* ijk,int coord_workspace[3])
{
	if(ijk==nullptr) return vtkImageData::GetScalarPointer();
	this->get3DcoordinateFromND(ijk,coord_workspace);
	return vtkImageData::GetScalarPointer(coord_workspace);
}

void* vtkHyperCube::GetScalarPointer(int* ijk)
{
	if(ijk==nullptr) return vtkImageData::GetScalarPointer();
	int ret= this->get3DcoordinateFromND(ijk,this->coord);
	return vtkImageData::GetScalarPointer(this->coord);
}

void* vtkHyperCube::GetScalarPointer(int x, int y, int z)
{
	  int tmp[3];
	  tmp[0] = x;
	  tmp[1] = y;
	  tmp[2] = z;
	  return vtkImageData::GetScalarPointer(tmp);
}

float vtkHyperCube::GetScalarComponentAsFloat(int* coords,int component,int coord_workspace[3])
{

	if(this->get3DcoordinateFromND(coords,coord_workspace)!=1){
		vtkErrorMacro("Error computing cooridnate")
		return 0.;
	}
	return vtkImageData::GetScalarComponentAsFloat(coord_workspace[0],
													coord_workspace[1],
													coord_workspace[2],
													component);
}

float vtkHyperCube::GetScalarComponentAsFloat(int* coords,int component)
{

	if(this->get3DcoordinateFromND(coords,this->coord)!=1){
		vtkErrorMacro("Error computing cooridnate")
		return 0.;
	}
	return vtkImageData::GetScalarComponentAsFloat(this->coord[0],
													this->coord[1],
													this->coord[2],
													component);
}

void vtkHyperCube::SetScalarComponentFromFloat(int* ijk,int component,float v,int coord_workspace[3])
{
	if(this->get3DcoordinateFromND(ijk,coord_workspace)!=1){
		vtkErrorMacro("Error computing coordinate")
		return;
	}
	return vtkImageData::SetScalarComponentFromFloat(coord_workspace[0],
														coord_workspace[1],
														coord_workspace[2],
														component,v);
}

void vtkHyperCube::SetScalarComponentFromFloat(int* ijk,int component,float v)
{
	if(this->get3DcoordinateFromND(ijk,this->coord)!=1){
		vtkErrorMacro("Error computing coordinate")
		return;
	}
	return vtkImageData::SetScalarComponentFromFloat(this->coord[0],
														this->coord[1],
														this->coord[2],
														component,v);
}

float vtkHyperCube::GetScalarComponentAsDouble(int* coords,int component,int coord_workspace[3])
{

	if(this->get3DcoordinateFromND(coords,coord_workspace)!=1){
		vtkErrorMacro("Error computing coordinate")
		return 0.;
	}
	return vtkImageData::GetScalarComponentAsDouble(coord_workspace[0],
													coord_workspace[1],
													coord_workspace[2],
													component);
}

float vtkHyperCube::GetScalarComponentAsDouble(int* coords,int component)
{

	if(this->get3DcoordinateFromND(coords,this->coord)!=1){
		vtkErrorMacro("Error computing coordinate")
		return 0.;
	}
	return vtkImageData::GetScalarComponentAsDouble(this->coord[0],
													this->coord[1],
													this->coord[2],
													component);
}

void vtkHyperCube::SetScalarComponentFromDouble(int* ijk,int component,double v,int coord_workspace[3])
{
	if(this->get3DcoordinateFromND(ijk,coord_workspace)!=1){
		vtkErrorMacro("Error computing coordinate")
		return;
	}
	return vtkImageData::SetScalarComponentFromDouble(coord_workspace[0],
														coord_workspace[1],
														coord_workspace[2],
														component,v);
}

void vtkHyperCube::SetScalarComponentFromDouble(int* ijk,int component,double v)
{
	if(this->get3DcoordinateFromND(ijk,this->coord)!=1){
		vtkErrorMacro("Error computing coordinate")
		return;
	}
	return vtkImageData::SetScalarComponentFromDouble(this->coord[0],
														this->coord[1],
														this->coord[2],
														component,v);
}

void vtkHyperCube::SetSpacing(double* spc)
{
	for(int i=0;i<this->NDimensions;i++){
		this->Spacing[i]=spc[i];
	}
	for(int i=this->NDimensions;i<3;i++){
		this->Spacing[i]=1.;
	}
	//vtkImageData::SetSpacing(this->Spacing.data());
}

void vtkHyperCube::SetOrigin(double* org)
{
	for(int i=0;i<this->NDimensions;i++){
		this->Origin[i]=org[i];
	}
	for(int i=this->NDimensions;i<3;i++){
		this->Origin[i]=0.;
	}
	//vtkImageData::SetSpacing(this->Spacing.data());
}

void* vtkHyperCube::GetArrayPointer(vtkDataArray* 	array,
							int* coordinates,
							int coord_workspace[3])
{
	this->get3DcoordinateFromND(coordinates,coord_workspace);
	return vtkImageData::GetArrayPointer(array,coord_workspace);
}

void* vtkHyperCube::GetArrayPointer(vtkDataArray* 	array,
							int* coordinates)
{
	this->get3DcoordinateFromND(coordinates,this->coord);
	return vtkImageData::GetArrayPointer(array,this->coord);
}


void* vtkHyperCube::GetArrayPointerForExtent(vtkDataArray* array, int extent[6])
{
	for(int i=0;i<this->NDimensions;i++){
		this->iPoint[i]=extent[2*i];
	}
	return this->GetArrayPointer(array,this->iPoint.data());
}

void vtkHyperCube::ComputeInternalExtent(int* intExt, int* tgtExt, int* bnds)
{
	if(this->NDimensions>3){
		vtkErrorMacro("Developer Error routine not supported "
				" for ND > 3 got "<< this->NDimensions)
	}
	vtkImageData::ComputeInternalExtent(intExt,tgtExt,bnds);
}


void vtkHyperCube::GetNDPointFromId(vtkIdType id, int *ijk)
{
	vtkIdType pp = id;
	int* dims= this->GetFullDimensions();
	int i=this->GetNDimensions();
	while (i>1){
		vtkIdType str=1;
		for(int ii=0;ii<i-1;ii++){str=str*dims[ii];}
		ijk[i-1]=(int)((pp)/str);
		pp=pp-(ijk[i-1])*str;
		i--;
	}
	ijk[0]=(int)pp;
}

void vtkHyperCube::GetNDPointFromId(vtkIdType id, int *ijk,int* work_dims)
{
	vtkIdType pp = id;
	this->GetFullDimensions(work_dims);
	int i=this->GetNDimensions();
	while (i>1){
		vtkIdType str=1;
		for(int ii=0;ii<i-1;ii++){str=str*work_dims[ii];}
		ijk[i-1]=(int)((pp)/str);
		pp=pp-(ijk[i-1])*str;
		i--;
	}
	ijk[0]=(int)pp;
}


/**************************************/
// Private methods

std::vector<int> vtkHyperCube::makeExtentsFromDims(int* dims)
{
	std::vector<int> ext;
	for(int i=0;i < this->NDimensions;i++){
		ext.push_back(0);
		ext.push_back(dims[i]-1);
	}
	return ext;
}

int vtkHyperCube::get3DcoordinateFromND(int* coordND,int* coord3D)
{


	for(int i=0;i<3;i++){
		if(i<this->NDimensions){
			coord3D[i]=coordND[i];
		}else{
			coord3D[i]=0;
		}
	}

	if(this->NDimensions>3){
		//TODO - add bounds checking here
		int* dims = this->GetFullDimensions();
		int add=0;
		int wrap=dims[2];
		for(int j=3;j<this->NDimensions;j++){
			add = add +wrap*coordND[j];
		}
		coord3D[2]=coord3D[2]+add;
	}

	return 1;
}

int vtkHyperCube::getNDcoordinateFrom3D(int* coord3D,int* coordND)
{
	// Get the grid position
	vtkIdType pp = vtkImageData::ComputePointId(coord3D);
	this->GetNDPointFromId(pp,coordND);

	return 1;
}

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

#ifndef SEISTK_CPP_DATATYPES_VTKHYPERCUBE_H_
#define SEISTK_CPP_DATATYPES_VTKHYPERCUBE_H_

// Includes
#include "vtkImageData.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#include <iostream>
#include <vector>

//! Macro to define pointer into hyper cube and run code
#define USEHYPERCUBE_POINTERMACRO(type,cube,i,j,k,pvar,code)	\
	{														\
		type* pvar = (type*) cube->GetScalarPointer(i,j,k);	\
		code												\
	}

//! Defines float pointer (if possible) and runs code
#define USEHYPERCUBE_FLOAT_POINTERMACRO(cube,i,j,k,pvar,code)	\
		if(cube->GetScalarType()==VTK_FLOAT){					\
		USEHYPERCUBE_POINTERMACRO(float,cube,i,j,k,pvar,code)	\
		}

//! Defines double pointer (if possible) and runs code
#define USEHYPERCUBE_DOUBLE_POINTERMACRO(cube,i,j,k,pvar,code)	\
		if(cube->GetScalarType()==VTK_DOUBLE){					\
		USEHYPERCUBE_POINTERMACRO(double,cube,i,j,k,pvar,code)	\
		}

//! Defines float or double pointer (if possible) and runs code
/*
 * This enables us to create code segments that can be accessed
 *  run with single or double datasets.
 */
#define USEHYPERCUBE_FLOATDOUBLE_POINTERMACRO(cube,i,j,k,pvar,code)	\
		USEHYPERCUBE_FLOAT_POINTERMACRO(cube,i,j,k,pvar,code)		\
		USEHYPERCUBE_DOUBLE_POINTERMACRO(cube,i,j,k,pvar,code)

//! Allows different code to be run for single or double
#define USEHYPERCUBE_FLOATDOUBLE_CODEBRANCH_MACRO(cube,CodeForFloats,CodeForDoubles) \
		if(cube->GetScalarType()==VTK_FLOAT){					\
			CodeForFloats										\
		}														\
		if(cube->GetScalarType()==VTK_DOUBLE){					\
			CodeForDoubles										\
		}

/*!
\brief Extends the vtkImageData object to handle and arbitrary number of dimensions

Summary
=======

Usage should be the same as vtkImage data for grids with fewer than 3 dimensions
for this with more the the outer dimensions are "wrapped" in to the Z axis
and convenience functions provided to retrieve axis properties.

As with VTK imageData this utilizes the concept of extents. On each axis, the extent
is defined by the index of the first point and the index of the last point. The extent
should be set before the "Scalars" are set or allocated. The Extent is stored in the
order (X1, .., XN). The data set extent does not have to start at (0,0,0). (0,0,0) is just
the extent of the origin. The first point (the one with Id=0) is at extent (Extent[0],Extent[2],...).

Rather than overriding the Extents for vtkImageData, instead we define a and internal
vector (FullExtent) which stores the extents for the ND cube. The first two dimensions
should be exactly the same as those for vtkImageData. The outer extents are then wrapped
into the 3rd dimension of the 3D image cube.


*/
class VTK_EXPORT vtkHyperCube : public vtkImageData
{
public:

	// New method
	static vtkHyperCube *New();

	// Typedef
	vtkTypeMacro(vtkHyperCube , vtkImageData)

	//! Print self
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Retrieve an instance of this class from an information object.
	static vtkHyperCube* GetData(vtkInformation* info);

	//! Retrieve an instance of this class from an information object.
	static vtkHyperCube* GetData(vtkInformationVector* v, int i=0);

	//! Set scalar name
	virtual void SetScalarName(std::string string){
		this->GetPointData()->GetScalars()->SetName(string.c_str());
	}

	//! Get scalar name
	virtual char* GetScalarName(){
		return this->GetPointData()->GetScalars()->GetName();
	}

	//! Perform shallow copy
	void ShallowCopy(vtkDataObject* src);

	//! Perform a deep copy
	void DeepCopy(vtkDataObject* src);

	//! Create an empty copy
	virtual void EmptyCopy(vtkDataObject* src);

	//! Copy but without allocating any scalars
	/*!
	 * Note - that if you use this you also have to set the ScalarName
	 */
	virtual void UnAllocatedCopy(vtkDataObject* src);

	//! Reallcates the cube preserving the original data
	/*!
	 * This can also be used to change the scalar type etc.
	 */
	int ReallocateScalars(int dataType, int numComponents,int Ndim, int* dims);

	//! Zero elements in double or float cube
	virtual void Zero();

	//! Retuns the size in bytes - assuming coniguous storage
	vtkIdType GetGridSizeInBytes()
	{
		return this->GetNumberOfPoints() *
				( this->GetNumberOfScalarComponents()* this->GetScalarSize());
	}

	//! Compute 3D (wrapped) coordinates from ND - thread safe
	void  Get3DcoordinateFromND(int* coordND, int coord3D[3]);

	//! Compute 3D (wrapped) coordinates from ND - Not thread safe
	int*  Get3DcoordinateFromND(int* coordND);

	//! Compute ND (unwrapped) coordinates from 3D - thread safe
	void  GetNDcoordinateFrom3D(int coord3D[3], int* iPoint);

	//! Compute ND (unwrapped) coordinates from 3D - Not thread safe
	int*  GetNDcoordinateFrom3D(int* coord3D);

	//! Compute ND (unwrapped) coordinates from 3D
	int*  GetNDcoordinateFrom3D(int i, int j, int k){
		int coord[3]={i,j,k};
		return this->GetNDcoordinateFrom3D(coord);
	}



	//!TODO Add an extra dimension - on outside

	//!TODO Remove Null axis with length 1


	/*************************************/
	/* Overriding vtkImageData functions */
	/*************************************/


	//! Get the number of dimensions
	vtkGetMacro(NDimensions,int)

	//! Set the number of dimensions
	void SetNDimensions(int N);

	// Override setting dimensions
	virtual void SetDimensions(int i, int j, int k);

	// Setting dimensions
	virtual void SetDimensions(int* dims);

	// Setting dimensions
	virtual void SetDimensions(int Ndim, int* dims);

	//! Getter for the full set of dimensions - use GetDimensions() for the wrapped 3D cube
	virtual int* GetFullDimensions();

	//! Getter for the full set of dimensions - use GetDimensions() for the wrapped 3D cube
	virtual void GetFullDimensions (int* dOut);

	//! Function to set the full extents - use GetExtent() for the wrapped 3D cube
	virtual void SetFullExtent(int* ext);

	//! Function to set the full extents -  - use GetExtent() for the wrapped 3D cube
	virtual int* GetFullExtent(){return this->FullExtent.data();}

	// Return data dimensionality
	virtual int GetDataDimension();

	//! Return number of cells in the full dataset (use GetNumberOfCells for the 3D wrapped object)
	vtkIdType GetFullNumberOfCells();

	//! Get point position overriding vtkImageData method - thread safe
	/*!
	 * returns
	 * =======
	 * ijk - the point coordinates
	 * work_dims - the dimensions of the cube
	 * xx - the position of the point in the grid
	 */
	void GetPoint(vtkIdType ptId ,int* work_ijk, int* work_dims, double* xx)
	{
		this->GetNDPointFromId(ptId,work_ijk,work_dims);
		int nn =this->GetNDimensions();
		double* spc= this->GetSpacing();
		double* org= this->GetOrigin();
		for(int i=0;i<nn;i++)xx[i]=org[i]+spc[i]*work_ijk[i];
	}

	//! Get point position overriding vtkImageData method - not  thread safe
	void GetPoint(vtkIdType ptId, int* work, double* xx)
	{
		this->GetNDPointFromId(ptId,work);
		int nn =this->GetNDimensions();
		double* spc= this->GetSpacing();
		double* org= this->GetOrigin();
		for(int i=0;i<nn;i++)xx[i]=org[i]+spc[i]*work[i];
	}

	//! Get point position (NOT thread safe)
	void GetPoint(vtkIdType ptId,double* xx)
	{
		this->GetNDPointFromId(ptId,this->iPoint.data());
		int nn =this->GetNDimensions();
		double* spc= this->GetSpacing();
		double* org= this->GetOrigin();
		for(int i=0;i<nn;i++)xx[i]=org[i]+spc[i]*this->iPoint[i];
	}

	//! Get point position (NOT thread safe)
	double* GetPoint(vtkIdType ptId)
	{
		this->GetNDPointFromId(ptId,this->iPoint.data());
		int nn =this->GetNDimensions();
		double* spc= this->GetSpacing();
		double* org= this->GetOrigin();
		for(int i=0;i<nn;i++)this->Point[i]=org[i]+spc[i]*this->iPoint[i];
		return this->Point.data();
	}

	//! Convenience function computes the structured coordinates for a point x[3]
	/*!
	 * The voxel is specified by the array ijk, and the parametric coordinates
	 *   in the cell are specified with pcoords[3].
	 *   The function returns a 0 if the point x is outside of the volume,
	 *   and a 1 if inside the volume.
	 */
	virtual int ComputeStructuredCoordinates(const double* x, int* ijk, double* pcoords);

	//! Computation of general structured coordinates - more general
	/*!
	 * Returns 0 if x is outside the volume, 1 if it is inside
	 */
	int ComputeStructuredCoordinates( const double x[3], int ijk[3],
												double pcoords[3],
												const int* extent,
												const double* spacing,
												const double* origin,
												const double* bounds);


	/*!
	 * Given structured coordinates (ijk) for a voxel cell, compute the eight gradient values
	 *  for the voxel corners. The order in which the gradient vectors are arranged corresponds
	 *  to the ordering of the voxel points. Gradient vector is computed by central
	 *  differences (except on edges of volume where forward difference is used). The scalars
	 *  s are the scalars from which the gradient is to be computed. This method will treat
	 *  only 3D structured point datasets (i.e., volumes).
	 *
	 *  Disabled for ND > 3
	 *
	 */
	virtual void GetVoxelGradient(int* ijk, vtkDataArray *s, vtkDataArray *g){
		if(this->NDimensions<3){
			vtkErrorMacro("vtkHyperCube::GetVoxelGradient not supported  for NDimension>3- developer error")
			return;
		}
		this->GetVoxelGradient(ijk[0],ijk[1],ijk[2],s,g);
	}

	/*!
	 * Given structured coordinates (ijk) for a voxel cell, compute the eight gradient values
	 *  for the voxel corners. The order in which the gradient vectors are arranged corresponds
	 *  to the ordering of the voxel points. Gradient vector is computed by central
	 *  differences (except on edges of volume where forward difference is used). The scalars
	 *  s are the scalars from which the gradient is to be computed. This method will treat
	 *  only 3D structured point datasets (i.e., volumes).
	 *
	 *  Disabled for ND > 3
	 *
	 */
	virtual void GetVoxelGradient(int i, int j, int k, vtkDataArray *s, vtkDataArray *g)
	{
		if(this->NDimensions<3){
			vtkErrorMacro("vtkHyperCube::GetVoxelGradient not supported  for NDimension>3- developer error")
			return;
		}
		vtkImageData::GetVoxelGradient(i,j,k,s,g);
	}

	/*!
	 * Given structured coordinates (ijk) for a point in a structured point dataset,
	 * 	compute the gradient vector from the scalar data at that point. The scalars s are the
	 * 	scalars from which the gradient is to be computed. This method will treat structured
	 * 	point datasets of any dimension.
	 *
	 * 	Disbaled for ND>3
	 *
	 */
	virtual void GetPointGradient(int* ijk, vtkDataArray *s, double* g){
		if(this->NDimensions<3){
			vtkErrorMacro("vtkHyperCube::GetVoxelGradient not supported  for NDimension>3- developer error")
			return;
		}
		this->GetPointGradient(ijk[0],ijk[1],ijk[2],s,g);
	}

	/*!
	 * Given structured coordinates (ijk) for a point in a structured point dataset,
	 * 	compute the gradient vector from the scalar data at that point. The scalars s are the
	 * 	scalars from which the gradient is to be computed. This method will treat structured
	 * 	point datasets of any dimension.
	 *
	 * 	Disbaled for ND>3
	 *
	 */
	virtual void GetPointGradient(int i, int j, int k, vtkDataArray *s, double* g)
	{
		if(this->NDimensions<3){
			vtkErrorMacro("vtkHyperCube::GetVoxelGradient not supported  for NDimension>3- developer error")
			return;
		}
		vtkImageData::GetPointGradient(i,j,k,s,g);
	}

	//! Given a location in structured coordinates (i-j-k), return the point id. - thread safe
	virtual vtkIdType ComputePointId (int* ijk,int coord_workspace[3]);

	//! Given a location in structured coordinates (i-j-k), return the point id. - not thread safe
	virtual vtkIdType ComputePointId (int* ijk);

	//! Given a location in structured coordinates (i-j-k), return the cell id. - thread safe
	virtual vtkIdType ComputeCellId(int* ijk,int coord_workspace[3]);

	//! Given a location in structured coordinates (i-j-k), return the cell id. - not thread safe
	virtual vtkIdType ComputeCellId(int* ijk);

	//! Update and axis extent - note this does not change the structure
	virtual void SetAxisUpdateExtent(int 	axis,
									int 	min,
									int 	max,
									const int * 	updateExtent,
									int * 	axisUpdateExtent
									);

	//! Update and axis extent - note this does not change the structure
	virtual void GetAxisUpdateExtent(int axis, int &min, int &max, const int* updateExtent);

	//! Set extent - short cut for using 3D cube
	virtual void SetExtent(int extent[6],bool Auto3D=true){
		if(Auto3D==true) this->NDimensions=3;
		vtkImageData::SetExtent(extent);
	}

	//! Set extent - short cut for using 3D cube
	virtual void SetExtent(int x1,int x2,int y1,int y2,int z1,int z2,bool Auto3D=true){
		if(Auto3D==true) this->NDimensions=3;
		vtkImageData::SetExtent(x1,x2,y1,y2,z1,z2);
	}

	//! Access the native pointer for the scalar data uses elements 0,2,... from the extent input - thread safe
	virtual void* GetScalarPointerForExtent	(int* extent, int* coord_workspace,int coord3d_workspace[3]);

	//! Access the native pointer for the scalar data uses elements 0,2,... from the extent input - not thread safe
	virtual void* GetScalarPointerForExtent	(int* extent);

	//! Access the native pointer for the scalar data - thread safe
	virtual void* GetScalarPointer(int* ijk,int coord_workspace[3]);

	//! Access the native pointer for the scalar data- not thread safe
	virtual void* GetScalarPointer(int* ijk=nullptr);

	//! Access the native pointer for the scalar data. - note assumes 3D wrapping applied to Z
	virtual void* GetScalarPointer(int x, int y, int z);

	//! Gets a scalar value - thread safe
	virtual float GetScalarComponentAsFloat(int* coords,int component,int coord_workspace[3]);

	//! Gets a scalar value - not thread safe
	virtual float GetScalarComponentAsFloat(int* coords,int component);

	//! Gets a scalar value from 3D grid - not thread safe
	virtual float GetScalarComponentAsFloat(int x,int y,int z,int component){
		return vtkImageData::GetScalarComponentAsFloat(x,y,z,component);
	}

	//! Sets a scalar value
	virtual void SetScalarComponentFromFloat(int x, int y,int z,int component,float v){
		return vtkImageData::SetScalarComponentFromFloat(x,y,z,component,v);
	}

	//! Sets a scalar value - thread safe
	virtual void SetScalarComponentFromFloat(int* ijk,int component,float v,int coord_workspace[3]);

	//! Sets a scalar value - not thread safe
	virtual void SetScalarComponentFromFloat(int* ijk,int component,float v);

	//! Gets a scalar value - thread safe
	virtual float GetScalarComponentAsDouble(int* coords,int component,int coord_workspace[3]);

	//! Gets a scalar value - not thread safe
	virtual float GetScalarComponentAsDouble(int* coords,int component);

	//! Gets a scalar value from 3D grid
	virtual double GetScalarComponentAsDouble(int x,int y,int z,int component){
		return vtkImageData::GetScalarComponentAsDouble(x,y,z,component);
	}

	//! Sets a scalar value - not threadsafe
	virtual void SetScalarComponentFromDouble(int x, int y,int z,int component,double v){
		return vtkImageData::SetScalarComponentFromDouble(x,y,z,component,v);
	}

	//! Sets a scalar value - thread safe
	virtual void SetScalarComponentFromDouble(int* ijk,int component,double v,int coord_workspace[3]);

	//! Sets a scalar value - not thread safe
	virtual void SetScalarComponentFromDouble(int* ijk,int component,double v);

	//! This method is disabled
	virtual void CopyAndCastFrom(vtkHyperCube* inData,int* 	extent){
		vtkErrorMacro("vtkHyperCube::CopyAndCastFrom - not supported  - Developer error")
		return;
	}

	//! Reallocates and copies to crop to an extent - disabled
	void Crop(const int* updateExtent){
		if(this->NDimensions<=3){
			return vtkImageData::Crop(updateExtent);
		}
		vtkErrorMacro("vtkHyperCube::Crop - not supported  - for HyperCubes with N dimension > 3")
		return;
	}

	//! Setting spacing
	virtual void SetSpacing(double dx, double dy, double dz)
	{
		double spc[3]={dx,dy,dz};
		this->SetSpacing(spc);
	}

	//! Setting spacing
	virtual void SetSpacing(double* spc);

	//! Getting spacing for a single axis
	//virtual PyObject* GetSpacingPython(){return (PyObject*)this->Spacing.data();}
	virtual double GetAxisSpacing(int Axis){return this->Spacing[Axis];}

	//! Getting spacing - note this does not seem to work from python
	virtual double* GetSpacing(){return this->Spacing.data();}

	//! Getter for the spacings - for 3D compatibility
	virtual void GetSpacing	(double &x, double &y, double &z){
		x=this->Spacing[0];
		y=this->Spacing[1];
		z=this->Spacing[2];
	}

	//! Getter for the spacings
	virtual void GetSpacing	(double *scp){
		scp=this->Spacing.data();
	}

	//! Set axis origins
	virtual void SetOrigin(double x, double y, double z){
		double org[3]={x,y,z};
		this->SetOrigin(org);
	}

	//! Set axis origins
	virtual void SetOrigin(double* );

	//! Get axis origins
	virtual double* GetOrigin(){return this->Origin.data();}

	//! Getting origin for a single axis
	virtual double GetAxisOrigin(int Axis){return this->Origin[Axis];}

	//! Convenience method for getting a pointer from any field array - thread safe
	void*  GetArrayPointer(vtkDataArray* 	array,int* coordinates, int coord_workspace[3]);

	//! Convenience method for getting a pointer from any field array - not threadsafe
	void*  GetArrayPointer(vtkDataArray* 	array,int* coordinates);

	//! Convenience method for getting a pointer from any field array - not threadsafe
	void *GetArrayPointerForExtent(vtkDataArray* array, int extent[6]);

	/*!
	 * Given how many pixel are required on a side for boundary conditions
	 *   (in bnds), the target extent to traverse,
	 *   compute the internal extent (the extent for this HyperCube
	 *   that does not suffer from any boundary conditions)
	 *   and place it in intExt.
	 *
	 *   disabled for Ndimension > 3
	 */
	void ComputeInternalExtent(int* intExt, int* tgtExt, int* bnds);


	//! Get point in ND structure coords based in id (note using GetPoint return the 3D version) - not thread safe
	void GetNDPointFromId(vtkIdType id, int *ijk);

	//! Get point in ND structure coords based in id - thread safe
	/*!
	 * returns
	 * =======
	 * ijk - the point coordinates
	 * work_dims - the dimensions of the cube
	 */
	void GetNDPointFromId(vtkIdType id, int *ijk,int * work_dims);

protected:

	// Constructor
	vtkHyperCube(){
		this->NDimensions=3;		// Default is to behave the same as a vtkImageData

		this->FullExtent.resize(6);
		this->FullExtent={0,-1,0,-1,0,-1};

		this->FullDimensions.resize(3,1);

		this->Spacing.resize(3,1.);
		this->Origin.resize(3,0.);

		// TODO- not thread safe
		this->Point.resize(3);
		this->iPoint.resize(3);
	}

	// Destructor
	//~vtkHyperCube(){}

	int NDimensions;					//!< Number of dimensions
	std::vector<int> FullExtent;		/*!< Full extents of the object (all dimensions)
												we use this instead of Extents bacause that is
												used to map the 3D grid.
											*/
	std::vector<int> FullDimensions; 	//!< Full dimensions of the object (all dimensions)
	std::vector<double> Spacing;		//!< Axis spacings
	std::vector<double> Origin;			//!< Axis origins

private:


	std::vector<double> Point;			//!< for the GetPoint method (Note - not thread safe)
	std::vector<int> iPoint;			//!< For computing positions (Note - not thread safe)
	int coord[3];						//!< for computing 3D positions (Note - not thread safe)
	//#if defined(_OPENMP)
	//	#pragma omp threadprivate(Point,iPoint,coord)
	//#endif


	//! Internal method to compute extents from input dimensions
	virtual std::vector<int> makeExtentsFromDims(int* dims);

	//! Returns the index of a point in 3D (warped) coordinates.
	int get3DcoordinateFromND(int* coordND,int* coord3D);

	//! Returns the index of a point in ND (unwarped) coordinates.
	int getNDcoordinateFrom3D(int* coord3D,int* coordND);

};


#endif /* SEISPROC_C___ALGORITHMS_VTKHYPERCUBE_H_ */

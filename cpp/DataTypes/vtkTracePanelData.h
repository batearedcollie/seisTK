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

#ifndef SEISTK_CPP_DATATYPES_VTKTRACEPANELDATA_H_
#define SEISTK_CPP_DATATYPES_VTKTRACEPANELDATA_H_

// Includes
#include "vtkHyperCube.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkSmartPointer.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include "vtkHeaderTable.h"

//! Definitions (vtk types go up to 36)
#define VTK_TRACE_PANEL_DATA 38


/*!

\brief Combines a vtkHyperCube and vtkTraceHeader Object inorder to store traces

By convention the X axis of the cube is trace time. Y is the trace number

Usage
=====

C++
---
	@code
		vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();

		int dims[2]={125,10};			// 10 traces of 125 samples
		trc->SetDimensions(dims);
		trc->AllocateScalars(VTK_FLOAT,1);

		double smpl[2]={0.004,1.};		// 4ms sample rate
		trc->SetSpacing(smpl);

		double org[2]={-1,0.};			// start of time axis at -1. seconds
		trc->SetOrigin(org);

		// Add some trace headers
		vtkSmartPointer<vtkTraceHeader> hdr= trc->GetHeaderTable();

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

		cout << "\nHeader table:\n";
		trc->GetHeaderTable()->Dump();

		// Write out the trace data
		cout << "\nTraceData object:\n";
		trc->Print(cout);
	@endcode

Python
------

	@code
    tpD = vtkTracePanelData()
    tpD.SetDimensions(nsmpl,ntrace,1)
    tpD.SetSpacing(delta,1.,1.)
    tpD.SetOrigin(origin,0.,0.)

    vtk_data = numpy_support.numpy_to_vtk(array.flatten(),
                                          deep=copy
                                          )
    vtk_data.SetName("TraceData")
    tpD.GetPointData().SetScalars(vtk_data)
	@endcode


*/
class VTK_EXPORT vtkTracePanelData : public vtkHyperCube
{
public:

	//! New method
	static vtkTracePanelData *New();

	//! Typedef
	vtkTypeMacro(vtkTracePanelData , vtkHyperCube)

	//! Print self
	/*!
	 * Note - this should probably not be called from inside a parallel region
	 */
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Friend function to access the header table
	vtkHeaderTable* GetTraceHeaderTable(){
		return this->Headers["traces"];
	}

	//! From vtkType.h, a handle on what type of vtkDataObject this is.
	int GetDataObjectType() {return VTK_TRACE_PANEL_DATA;}

	//! Retrieve an instance of this class from an information object.
	static vtkTracePanelData* GetData(vtkInformation* info);

	//! Retrieve an instance of this class from an information object.
	static vtkTracePanelData* GetData(vtkInformationVector* v, int i=0);

	//! Shallow copy the data from src into this object.
	virtual void ShallowCopy(vtkDataObject* src);

	//! Deep copy
	virtual void DeepCopy(vtkDataObject* src);

	//! Create an copy
	virtual void EmptyCopy(vtkDataObject* src);

	//! Copy but without allocating any scalars
	virtual void UnAllocatedCopy(vtkDataObject* src);

	//! Check Header table exists
	bool HeaderExists(const char* key){
		if(this->Headers.count(key)!=1) return false;
		return true;
	}

	//! Get list of Aux header tables
	std::vector<std::string> GetHeaderTableNames(){
		std::vector<std::string> out;
		for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
			this->Headers.begin(); it != this->Headers.end(); ++it) {
			out.push_back(it->first);
		}
		return out;
	}

	//! Get Auxilary Header Table
	vtkHeaderTable* GetHeaderTable(const char* key){
		return this->Headers[key];
	}

	//! Remove Auxilary Header Table
	void RemoveHeaderTable(const char* key){
		this->Headers.erase(key);
	}

	//! Add Blank Auxilary Header Table
	vtkHeaderTable* AddBlankHeaderTable(const char* key)
	{
		if(this->HeaderExists(key)==true){
			vtkErrorMacro("Header table " << key << " already exists")
			return nullptr;
		}

		this->Headers[key] = vtkSmartPointer<vtkHeaderTable>::New();
		return this->Headers[key];
	}


protected:

	vtkTracePanelData(){
		this->SetNDimensions(2);
		this->Headers["traces"] = vtkSmartPointer<vtkHeaderTable>::New();
	}

	~vtkTracePanelData(){
	}

private:


	std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>> Headers; //!< Additional Header tables
																			/*!
																			 * Can be used to store trace picks etc.
																			 */

};


#endif /* SEISTK_CPP_DATATYPES_VTKTRACEPANELDATA_H_ */

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

#ifndef SEISTK_CPP_DATATYPES_VTKTRACEHEADER_H_
#define SEISTK_CPP_DATATYPES_VTKTRACEHEADER_H_

// Includes
#include "vtkTable.h"

#include "vtkSmartPointer.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkVariant.h"
#include "vtkVariantArray.h"
#include "vtkIntArray.h"
#include "vtkDataSetAttributes.h"
#include "vtkDataArray.h"

#include <string>
#include <iostream>

//! Definitions (vtk types go up to 40)
#define VTK_TRACE_HEADER 91

/*!

\brief Stores values for Trace headers as a table

Usage
=====

C++
---

	@code

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

	@endcode

Python
------
	@code
	from stk.DataTypes import vtkTraceHeader

    header = vtkHeaderTable()

    # Add stations field
    v = vtk.vtkVariantArray()
    v.SetName("station")
    for i in range(0,10): v.InsertNextValue("stn-"+str(i))
    header.AddColumn(v)

    # Add channels field
    v = vtk.vtkVariantArray()
    v.SetName("channel")
    for i in range(0,10): v.InsertNextValue("chn-"+str(i))
    header.AddColumn(v)

    # Set a uniform value
    header.SetUniformValue("dt",vtk.vtkVariant(0.004))

    # Dump to screen
    header.Dump()
	@endcode
*/
class VTK_EXPORT vtkHeaderTable : public vtkTable
{

public:
	//! New method
	static vtkHeaderTable *New();

	//! Typedef
	vtkTypeMacro(vtkHeaderTable,vtkTable)

	//! Printself
	void PrintSelf(ostream &os, vtkIndent indent);

	//! Set constant dictionary value
	void SetUniformValue(const char* key, vtkVariant value){
		if(this->KeyExists(key)==true){
			for(int i=0;i<this->GetNumberOfRows();i++){
				this->SetValueByName(i,key,value);
			}
		}else{
			vtkSmartPointer<vtkVariantArray> vv = vtkSmartPointer<vtkVariantArray>::New();
			for(int i=0;i<this->GetNumberOfRows();i++){
				vv->InsertNextValue(value);
			}
			vv->SetName(key);
			this->AddColumn(vv);
		}
	}

	//! Set the a constant tuple value
	template<typename T, typename V>
	void SetUniformTuple(const char* key, const int size, T* tuple)
	{
		if(this->KeyExists(key)==true){
			vtkSmartPointer<V> vv = (V*) this->GetColumnByName(key);
			for(int i=0;i<this->GetNumberOfRows();i++){
				vv->SetTypedTuple(i,tuple);
			}
		}else{
			vtkSmartPointer<V> vv = vtkSmartPointer<V>::New();
			vv->SetName(key);
			vv->SetNumberOfComponents(size);
			for(int i=0;i<this->GetNumberOfRows();i++){
				vv->InsertNextTuple(tuple);
			}
			this->AddColumn(vv);
		}

	}

	//! From vtkType.h, a handle on what type of vtkDataObject this is.
	int GetDataObjectType() {return VTK_TRACE_HEADER;}

	//! Retrieve an instance of this class from an information object.
	static vtkHeaderTable* GetData(vtkInformation* info);

	//! Retrieve an instance of this class from an information object.
	static vtkHeaderTable* GetData(vtkInformationVector* v, int i=0);

	//! Shallow copy the data from src into this object.
	virtual void ShallowCopy(vtkDataObject* src);

	//! Deep copy
	virtual void DeepCopy(vtkDataObject* src);

	//! Empty copy - just copies the column headers
	virtual void EmptyCopy(vtkDataObject* src);

	//! Returns a list of column headers / keys
	std::vector<std::string> GetKeyList(){
		vtkSmartPointer<vtkDataSetAttributes> attr = this->GetRowData();
		std::vector<std::string> ret;
		for(int i=0;i<attr->GetNumberOfArrays();i++){
			const char* cc = attr->GetAbstractArray(i)->GetName();
			ret.push_back( std::string (cc ) );
		}
		return ret;
	}

	//! Returns true is the key is already present in the table
	bool KeyExists(const char* kk){
		std::string ss(kk);
		std::vector<std::string> keyList = this->GetKeyList();
		bool ret =false;
		for(int i=0;i<keyList.size();i++){
			if(keyList[i]==ss){
				ret=true;
				break;
			}
		}
		return ret;
	}

	//! Adds an id field to a table
	int AddIdField();

	//! Get the maximum value from a field
	vtkVariant GetColumnMaxVal(const char* col);

protected:

	//! Constructor
	vtkHeaderTable(){
	}

	//! Destructor
	~vtkHeaderTable(){}


private:

};


#endif /* CPP_DATATYPES_VTKTRACEHEADER_H_ */

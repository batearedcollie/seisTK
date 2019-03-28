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
#include "vtkObjectFactory.h"

#include "vtkHeaderTable.h"

#include "vtkStringArray.h"

/**************************************/
// Definitions

using namespace std;


/**************************************/
// Public methods

// New method
vtkStandardNewMacro(vtkHeaderTable)

// Print self
void vtkHeaderTable::PrintSelf(ostream& os, vtkIndent indent)
{
	os << indent << "vtkHeaderTable:\n";
	this->Superclass::PrintSelf(os,indent.GetNextIndent());
	os << indent << "End vtkHeaderTable" << endl;
}

// Get Data
vtkHeaderTable* vtkHeaderTable::GetData(vtkInformation* info)
{
  return info? vtkHeaderTable::SafeDownCast(info->Get(DATA_OBJECT())) : 0;
}

// Get Data
vtkHeaderTable* vtkHeaderTable::GetData(vtkInformationVector* v, int i)
{
	return vtkHeaderTable::GetData(v->GetInformationObject(i));
}

// Shallow copy
void vtkHeaderTable::ShallowCopy(vtkDataObject* src)
{
	this->Superclass::ShallowCopy(src);
}

//Deep
void vtkHeaderTable::DeepCopy(vtkDataObject* src)
{
	this->Superclass::DeepCopy(src);
}

//Empty
void vtkHeaderTable::EmptyCopy(vtkDataObject* src)
{

	if (vtkHeaderTable* const pdo = vtkHeaderTable::SafeDownCast(src))
    {
		this->Initialize();
		std::vector<std::string> keys = pdo->GetKeyList();
		for(int i=0;i<keys.size();i++){
			vtkSmartPointer<vtkVariantArray> vv = vtkSmartPointer<vtkVariantArray>::New();
			vv->SetName(keys[i].c_str());
			this->AddColumn(vv);
		}
		this->Modified();
    }
}

int vtkHeaderTable::AddIdField()
{
	if(this->KeyExists("id")==false){
		vtkSmartPointer<vtkIntArray> vv = vtkSmartPointer<vtkIntArray>::New();
		vv->SetName("id");
		for(int i=1;i<=this->GetNumberOfRows();i++){
			vv->InsertNextValue(i);
		}
		this->AddColumn(vv);
	}else{
		int vv = this->GetColumnMaxVal("id").ToInt();
		for(vtkIdType ii=0;ii<this->GetNumberOfRows();ii++){
			vtkVariant vi = this->GetValueByName(ii,"id");
			if(vi.IsValid()==false){
				vv++;
				this->SetValueByName(ii,"id",vv);
			}
		}
	}

	return 1;
}

vtkVariant vtkHeaderTable::GetColumnMaxVal(const char* col)
{
	vtkSmartPointer<vtkVariantArray> vv = vtkVariantArray::SafeDownCast( this->GetColumnByName(col) );

	vtkVariant vout;
	for(vtkIdType i=0;i<vv->GetNumberOfValues();i++){
		vtkVariant vt = vv->GetValue(i);
		if(vt.IsValid()==true){
			if(vout.IsValid()==false){
				vout = vt;
			}else{
				if(vt>vout){
					vout = vt;
				}
			}
		}
	}
	return vout;
}

/**************************************/
// Protected methods

// Constructor

// Destructor



/**************************************/
// Private methods



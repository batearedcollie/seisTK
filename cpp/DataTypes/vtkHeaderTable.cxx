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
	std::vector<std::string> keys = this->GetKeyList();

	this->Initialize();

	for(int i=0;i<keys.size();i++){
		vtkSmartPointer<vtkVariantArray> vv = vtkSmartPointer<vtkVariantArray>::New();
		vv->SetName(keys[i].c_str());
		this->AddColumn(vv);
	}
}

/**************************************/
// Protected methods

// Constructor

// Destructor



/**************************************/
// Private methods



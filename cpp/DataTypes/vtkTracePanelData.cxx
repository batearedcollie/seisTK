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
#include "vtkTracePanelData.h"

/**************************************/
// Definitions

using namespace std;

/**************************************/
// Public methods

// New method
vtkStandardNewMacro(vtkTracePanelData)

// Print Self
void vtkTracePanelData::PrintSelf(ostream& os, vtkIndent indent)
{

	os << indent << "vtkTracePanelData:" << endl;

	for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
		this->Headers.begin(); it != this->Headers.end(); ++it) {
		os << indent << indent << "header table: " << it->first << endl;
		this->Headers[it->first.c_str()]->PrintSelf(os,indent.GetNextIndent().GetNextIndent());
		os << indent << indent << "End header table: " << it->first << endl;
	}


	this->Superclass::PrintSelf(os, indent.GetNextIndent());
	os << indent << "End vtkTracePanelData" << endl;
}

// Get Data
vtkTracePanelData* vtkTracePanelData::GetData(vtkInformation* info)
{
  return info? vtkTracePanelData::SafeDownCast(info->Get(DATA_OBJECT())) : 0;
}

// Get Data
vtkTracePanelData* vtkTracePanelData::GetData(vtkInformationVector* v, int i)
{
	return vtkTracePanelData::GetData(v->GetInformationObject(i));
}


// Shallow copy
void vtkTracePanelData::ShallowCopy(vtkDataObject* src)
{
	if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
    {
		for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
			this->Headers.begin(); it != this->Headers.end(); ++it) {
			this->AddBlankHeaderTable(it->first.c_str(),true);
			this->Headers[it->first.c_str()]->ShallowCopy(pdo->GetHeaderTable(it->first.c_str()));
		}
		this->Modified();
    }
	this->Superclass::ShallowCopy(src);
}

// Deep
void vtkTracePanelData::DeepCopy(vtkDataObject* src)
{
	if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
	{
		for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
			this->Headers.begin(); it != this->Headers.end(); ++it) {
			this->AddBlankHeaderTable(it->first.c_str(),true);
			this->Headers[it->first.c_str()]->DeepCopy(pdo->GetHeaderTable(it->first.c_str()));
		}
		this->Modified();
	}
	this->Superclass::DeepCopy(src);
}

// Empty copy
void vtkTracePanelData::EmptyCopy(vtkDataObject* src)
{
	if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
	{
		for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
			this->Headers.begin(); it != this->Headers.end(); ++it) {
			this->AddBlankHeaderTable(it->first.c_str(),true);
			this->Headers[it->first.c_str()]->EmptyCopy(pdo->GetHeaderTable(it->first.c_str()));
		}
		this->Modified();
    }
	this->Superclass::EmptyCopy(src);
}


// UnAllocated copy
void vtkTracePanelData::UnAllocatedCopy(vtkDataObject* src)
{
	if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
	{
		for(std::unordered_map<std::string,vtkSmartPointer<vtkHeaderTable>>::iterator it =
			this->Headers.begin(); it != this->Headers.end(); ++it) {
			this->AddBlankHeaderTable(it->first.c_str(),true);
			this->Headers[it->first.c_str()]->EmptyCopy(pdo->GetHeaderTable(it->first.c_str()));
		}
		this->Modified();
    }
	this->Superclass::UnAllocatedCopy(src);
}

/**************************************/
// Protected methods


/**************************************/
// Private methods

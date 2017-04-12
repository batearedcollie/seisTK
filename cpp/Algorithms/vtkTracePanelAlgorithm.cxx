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
#include "vtkDemandDrivenPipeline.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkTracePanelAlgorithm.h"




#include <iostream>

/**************************************/
// Definitions

using namespace std;

/**************************************/
// Public methods


// New method
vtkStandardNewMacro(vtkTracePanelAlgorithm)

// Print self
void vtkTracePanelAlgorithm::PrintSelf(ostream& os, vtkIndent indent)
{
	//TODO - add custom stuff here
	this->Superclass::PrintSelf(os, indent);
}


/**************************************/
// Protected


// Filling output ports
int vtkTracePanelAlgorithm::FillInputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
	info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkTracePanelData");
	return 1;
}

// Filling output ports
int vtkTracePanelAlgorithm::FillOutputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkTracePanelData");
  return 1;
}

int vtkTracePanelAlgorithm::RequestDataObject(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector),
  vtkInformationVector* vtkNotUsed(outputVector))
{
  vtkTracePanelData *newobj = vtkTracePanelData::New();
  this->GetExecutive()->SetOutputData(0, newobj);
  newobj->Delete();
  return 1;
}

/**************************************/
// Private methods


/**************************************/
// Private methods

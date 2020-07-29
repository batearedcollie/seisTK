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
#include "vtkHyperCubeAlgorithm.h"

/**************************************/
// Definitions

using namespace std;

/**************************************/
// Public methods

// New method
vtkStandardNewMacro(vtkHyperCubeAlgorithm)

// Print self
void vtkHyperCubeAlgorithm::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}


// Process request
int vtkHyperCubeAlgorithm::ProcessRequest(vtkInformation* request,
                                         vtkInformationVector** inputVector,
                                         vtkInformationVector* outputVector)
{
	// execute information
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION())){
	    return this->RequestInformation(request, inputVector, outputVector);
	}

	// Request update extent
	if(request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT())){
		return this->RequestUpdateExtent(request, inputVector, outputVector);
    }

	// generate the data
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA())){
		return this->RequestData(request, inputVector, outputVector);
    }

	// create the output
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA_OBJECT())){
		return this->RequestDataObject(request, inputVector, outputVector);
    }
	return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}


/**************************************/
// Protected

// Filling output ports
int vtkHyperCubeAlgorithm::FillInputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkHyperCube");
  return 1;
}

// Filling output ports
int vtkHyperCubeAlgorithm::FillOutputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkHyperCube");
  return 1;
}


// Request update extent
int vtkHyperCubeAlgorithm::RequestUpdateExtent(
	  vtkInformation* vtkNotUsed(request),
	  vtkInformationVector** inputVector,
	  vtkInformationVector* vtkNotUsed(outputVector))
{
	int numInputPorts = this->GetNumberOfInputPorts();
	for (int i=0; i<numInputPorts; i++){
		int numInputConnections = this->GetNumberOfInputConnections(i);
	    for (int j=0; j<numInputConnections; j++){
	        vtkInformation* inputInfo = inputVector[i]->GetInformationObject(j);
	        int ext[6];
	        inputInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),ext);
	        inputInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(),ext,6);
	    }
	}
	return 1;
}

// Request data object
int vtkHyperCubeAlgorithm::RequestDataObject(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector),
  vtkInformationVector* vtkNotUsed(outputVector))
{
  vtkHyperCube *newobj = vtkHyperCube::New();
  this->GetExecutive()->SetOutputData(0, newobj);
  newobj->Delete();
  return 1;
}


/**************************************/
// Private methods

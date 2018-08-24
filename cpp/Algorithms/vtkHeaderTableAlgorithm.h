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

#ifndef SEISTK_ALGORITHMS_VTKTRACEHEADERALGORITHM_H_
#define SEISTK_ALGORITHMS_VTKTRACEHEADERALGORITHM_H_

// Includes
#include "vtkHeaderTable.h"
#include "vtkTableAlgorithm.h"
#include "vtkSmartPointer.h"



/*!
\brief Abstract base algorithm class for working with trace header data.

*/
class VTK_EXPORT vtkHeaderTableAlgorithm : public vtkTableAlgorithm
{

public:

	//! New method
	static vtkHeaderTableAlgorithm *New();

	//! Typedef
	vtkTypeMacro(vtkHeaderTableAlgorithm , vtkTableAlgorithm)

	//! Print self
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Interface the algorithm to the Pipeline's passes.
	//int ProcessRequest(vtkInformation*,
	//                             vtkInformationVector**,
	//                            vtkInformationVector*);

	//! Return pointer to output data
	vtkHeaderTable* GetOutput(int port=0){
		return vtkHeaderTable::SafeDownCast(this->GetOutputDataObject(port));
	}

protected:

	vtkHeaderTableAlgorithm(){
		this->SetNumberOfInputPorts(1);
		this->SetNumberOfOutputPorts(1);
	}

	//! Fill input ports - to take vtkTracePanelData objects
	virtual int FillInputPortInformation(int port, vtkInformation* info);

	//! Fill output ports - to take vtkTracePanelData objects
	virtual int FillOutputPortInformation(int port, vtkInformation* info);

//	//! Request update extent
//		virtual int RequestUpdateExtent(
//		  vtkInformation* vtkNotUsed(request),
//		  vtkInformationVector** inputVector,
//		  vtkInformationVector* vtkNotUsed(outputVector));

	//! Produce output of proper type
	virtual int RequestDataObject(vtkInformation* request,
	                                vtkInformationVector** inputVector,
	                                vtkInformationVector* outputVector);

	//! Method that does the actual calculation. - should be overridden by children
	virtual int RequestData(vtkInformation* request,
	                          vtkInformationVector** inputVector,
	                          vtkInformationVector* outputVector){
		vtkErrorMacro("Developer Error RequestData not implemented")
		return 0;
	}
private:



};


#endif /* SEISTK_ALGORITHMS_VTKTRACEHEADERALGORITHM_H_ */

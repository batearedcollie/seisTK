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
#include <boost/python.hpp>

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#include <string>
#include <iostream>

// Definitions (vtk types go up to 36)
#define VTK_TRACE_PANEL_DATA 38


/*!

\brief Stores a set of traces as a 2D ImageData/hyper cube
 with a list of python dictionaries to store individual
 trace headers.

By convention the X axis of the cube is trace time. Y is the trace number

Usage
=====

C++
---
	@code
	// Create with an empty dictionary
	vtkSmartPointer<vtkTracePanelData> trc = vtkSmartPointer<vtkTracePanelData>::New();
	trc->SetDimensions(32,1,1);
	trc->AllocateScalars(VTK_FLOAT,1);
	trc->SetSpacing(0.004,1,1);
	trc->SetOrigin(0,0.,0.);
	@endcode

Python
------

	@code
    tpD = vtkTracePanelData()

    # Set the dimensions
    tpD.SetDimensions(nsmpl,ntrace,1)

    # Set the spacings
    tpD.SetSpacing(dfA.result["data"][0].stats.delta,1.,1.)

    # Set the origin
    tpD.SetOrigin(0.,0.,0.)

    # Set the data values
    points = tpD.GetPointData()
    points.SetScalars(vtk_data)

    # Set the dictionary
    tpD.setDict(dfA.result["data"][0].stats.__dict__)

    # Print out some information here
    print "Dimensions: ",tpD.GetDimensions()
    print "Number of points: ",tpD.GetNumberOfPoints()
    print "Number of cells: ",tpD.GetNumberOfCells()
    print "Data Array:",tpD.GetPointData().GetScalars("TraceData")
    dd = tpD.getDict()
    print "Trace dictionary"
    for key in dd: print key,":",dd[key]
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
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Prints out the trace dictionary
	void PrintTraceDictionary(ostream& os,int Trace);

	//! Prints out the complete trace dictionary
	void PrintFullTraceDictionary(ostream& os);

	//! Get full dictionary list -
	/*
	 *  NOTE - this is not safe when called from python beacuse it does bad
	 *  		this wrt whether Python or C handles the memory allocation for the list.
	 *  		For calling from python use.
	 */
	PyObject* GetDictionaryList(){
		return (PyObject*)this->d_list.ptr();
	}

	//! Dictionary list getter
	/*
	 * Return list should be a python list object
	 * to which the trace dictionaries will be added
	 */
	void GetDictionaryList(PyObject* ReturnList){
		boost::python::extract< boost::python::list > lout_ext(ReturnList);
		boost::python::list lout = lout_ext();
		boost::python::extract< boost::python::list > list_ext(this->d_list);
		boost::python::list ll = list_ext();
		int len = boost::python::len(ll);
		for(int i=0;i<len;i++){
	  		boost::python::extract<boost::python::dict> dict_ext(ll[i]);
	  		boost::python::dict dd = dict_ext().copy();
	  		lout.append(dd);
		}
	}

	//! Dictionary getter - returns Pyobject because of the VTK Parser
	/*
	 * note calling this function from python destroys the dictionary
	 * in C++. If you want to preserve the data source in c++
	 * use void GetDictionaryList(PyObject* ReturnList);
	 */
	PyObject* GetDict(int trace=0){
		boost::python::extract< boost::python::dict > dict_ext(this->d_list[trace]);
		boost::python::dict dd = dict_ext();
		return (PyObject*)dd.ptr();
	}

	//! Get a value from a trace dictionary header
	template<typename T>
	bool GetFromTraceDictionary(const char* key, T &ss, int Trace=0){
		boost::python::extract< boost::python::dict > dict_ext(this->d_list[Trace]);
		boost::python::dict dd = dict_ext();
		if(dd.has_key(key)==true){
			ss = boost::python::extract< T >( dd[key] );
			return true;
		}
		return false;
	}

	//! Removes item from all trace dictionaries in list
	void RemoveFromAllTraceDictionaries(const char* key);

	//! Removes a value from a trace header dictionary
	void RemoveFromTraceDictionary(const char* key, int Trace=0);

	//! Append to the dictionary list
	void appendDict(PyObject *dd);

	//! Update the trace dictionary for a specific trace
	void UpdateTraceDictionary(int trace, boost::python::dict);

	//! set or add an item to the dictionary on all traces
	template<typename T>
	void SetUniformDictionaryValue(const char* key, T val){
		boost::python::extract< boost::python::list > list_ext(this->d_list);
		boost::python::list ll = list_ext();
		int len = boost::python::len(ll);
		boost::python::dict dUpdate;
		dUpdate.clear();
		dUpdate[key]=val;
		for(int i=0;i<len;i++){
			this->UpdateTraceDictionary(i,dUpdate);
		}
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

	//! Copies a list of dictionaries into the trace dictionary list
	virtual void DictionaryListCopy(PyObject *dd);

	//! Copies a list of dictionaries into the trace dictionary list
	virtual void CleanDictionaryList();


protected:

	vtkTracePanelData(){
		this->SetNDimensions(2);
	}

	~vtkTracePanelData(){
	}

private:

	//! Trace Dictionary list
	boost::python::list d_list;
};


#endif /* SEISTK_CPP_DATATYPES_VTKTRACEPANELDATA_H_ */

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
	this->PrintFullTraceDictionary(os);
	os << indent << "End vtkTracePanelData" << endl;

	this->Superclass::PrintSelf(os, indent.GetNextIndent());
}

void vtkTracePanelData::PrintTraceDictionary(ostream& os, int Trace)
{
//	#if defined(_OPENMP)
//	#pragma omp critical (seistk_omp_critical)
//	{
//	#endif

	boost::python::extract< boost::python::dict > dict_ext(this->d_list[Trace]);
	if(!dict_ext.check()){
        vtkErrorMacro("Trace " << Trace <<"  has a bad dictionary")
    }
    boost::python::dict dd = dict_ext();
	boost::python::list keylist = dd.keys();
	int lk = boost::python::len(keylist);
    os << "Trace " << Trace << endl;
	for(int j=0;j<lk;j++){
      std::string keystr = boost::python::extract< std::string >( boost::python::str(keylist[j]));
      std::string valstr = boost::python::extract< std::string >( boost::python::str(dd[keylist[j]]));
      os << "  key: " << keystr << " -> " << valstr << " " << endl;
	}

//	#if defined(_OPENMP)
//	}
//	#endif
}

void vtkTracePanelData::PrintFullTraceDictionary(ostream& os)
{

	int len;

//	#if defined(_OPENMP)
//	#pragma omp critical (seistk_omp_critical)
//	{
//	#endif

	boost::python::extract< boost::python::list > list_ext(this->d_list);
	boost::python::list ll = list_ext();
	len = boost::python::len(ll);
	os << "Full trace dictionary - Number of Traces: " << len << endl;

	for(int i1=0;i1<len;i1++){
		this->PrintTraceDictionary(os,i1);

//		boost::python::extract< boost::python::dict > dict_ext(this->d_list[i1]);
//		if(!dict_ext.check()){
//	        vtkErrorMacro("Trace " << i1 <<"  has a bad dictionary")
//	    }
//	    boost::python::dict dd = dict_ext();
//		boost::python::list keylist = dd.keys();
//		int lk = boost::python::len(keylist);
//	    os << "Trace " << i1 << endl;
//		for(int j=0;j<lk;j++){
//	      std::string keystr = boost::python::extract< std::string >( boost::python::str(keylist[j]));
//	      std::string valstr = boost::python::extract< std::string >( boost::python::str(dd[keylist[j]]));
//	      os << "  key: " << keystr << " -> " << valstr << " " << endl;
//		}

	}
	os << "End Dictionary " << endl;

//	#if defined(_OPENMP)
//	}
//	#endif

}


// Dictionary setter
void vtkTracePanelData::appendDict(PyObject *dd)
{
	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif
	boost::python::extract< boost::python::dict > dict_ext(dd);
	this->d_list.append(dict_ext().copy());
	#if defined(_OPENMP)
	}
	#endif
}

void vtkTracePanelData::UpdateTraceDictionary(int trace, boost::python::dict dUpdate)
{
	boost::python::extract< boost::python::dict > dict_ext(this->d_list[trace]);
	boost::python::dict dd = dict_ext();
	dd.update(dUpdate);
}

void vtkTracePanelData::RemoveFromAllTraceDictionaries(const char* key)
{
	int len;
	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif
	boost::python::extract< boost::python::list > list_ext(this->d_list);
	if(!list_ext.check()){
	    vtkErrorMacro("Bad dictionary list")
	}
	boost::python::list ll = list_ext();
	len = boost::python::len(ll);

	#if defined(_OPENMP)
	}
	#endif

	for(int i=0;i<len;i++){
		this->RemoveFromTraceDictionary(key,i);
	}

}

void vtkTracePanelData::RemoveFromTraceDictionary(const char* key, int Trace)
{
	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif

	boost::python::extract< boost::python::dict > dict_ext(this->d_list[Trace]);
	boost::python::dict dd = dict_ext();
	dd[key].del();

	#if defined(_OPENMP)
	}
	#endif
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
	  this->DictionaryListCopy(pdo->GetDictionaryList());
	  this->Modified();
    }

  this->Superclass::ShallowCopy(src);
}

//Deep
void vtkTracePanelData::DeepCopy(vtkDataObject* src)
{
if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
	{
		this->DictionaryListCopy(pdo->GetDictionaryList());
		this->Modified();
	}
	this->Superclass::DeepCopy(src);
}

//Empty copy
void vtkTracePanelData::EmptyCopy(vtkDataObject* src)
{
  if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
    {
	  this->DictionaryListCopy(pdo->GetDictionaryList());
      this->Modified();
    }
  this->Superclass::EmptyCopy(src);
}

void vtkTracePanelData::UnAllocatedCopy(vtkDataObject* src)
{
	if (vtkTracePanelData* const pdo = vtkTracePanelData::SafeDownCast(src))
	    {
		  this->DictionaryListCopy(pdo->GetDictionaryList());
	      this->Modified();
	    }
	this->Superclass::UnAllocatedCopy(src);
}

void vtkTracePanelData::DictionaryListCopy(PyObject *dd)
{

	this->CleanDictionaryList();

	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif

	boost::python::extract< boost::python::list > list_ext(dd);
    if(!list_ext.check()){
        vtkErrorMacro("Bad dictionary list")
    }
    boost::python::list ll = list_ext();
    int len = boost::python::len(ll);

    for(int i=0;i<len;i++){
  		boost::python::extract<boost::python::dict> dict_ext(ll[i]);
  		boost::python::dict dd = dict_ext().copy();
  		this->d_list.append(dd);
    }

    #if defined(_OPENMP)
    }
    #endif

	this->Modified();
}

void vtkTracePanelData::CleanDictionaryList()
{
	#if defined(_OPENMP)
	#pragma omp critical (seistk_omp_critical)
	{
	#endif

	boost::python::extract< boost::python::list > list_ext(this->d_list);
	boost::python::list ll = list_ext();
	int len = boost::python::len(ll);
	for(int i=0;i<len;i++){ll.pop();}

	#if defined(_OPENMP)
	}
	#endif
}


/**************************************/
// Protected methods


/**************************************/
// Private methods

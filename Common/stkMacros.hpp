/*
Copyright 2019 Bateared Collie

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

#ifndef CPP_COMMON_STKMACROS_HPP_
#define CPP_COMMON_STKMACROS_HPP_

//Includes
//#include "vtkMacros.h"

//! A generic error message macro which can be used without a vtk object
#define stkGenericWarningMacro(msg)									\
  if (vtkObject::GetGlobalWarningDisplay())                         \
  {                                                                 \
    vtkOStreamWrapper::EndlType endl;                               \
    vtkOStreamWrapper::UseEndl(endl);                               \
    vtkOStrStreamWrapper vtkmsg;                                    \
    vtkmsg << "ERROR: In " __FILE__ ", line " << __LINE__ << " - " <<  msg << endl; \
    vtkOutputWindowDisplayErrorText(vtkmsg.str());					\
    vtkmsg.rdbuf()->freeze(0); 										\
	vtkObject::BreakOnError();           							\
	}

//! Custom version of vtkAbstractTypeMacroWithNewInstanceType
#define stkAbstractTypeMacroWithNewInstanceType(thisClass,superclass,instanceType,thisClassName) 	\
  protected: 																						\
  const char* GetClassNameInternal() const override 												\
  { 																								\
    return thisClassName; 																			\
  } 																								\
  public: 																							\
  typedef superclass Superclass; 																	\
  static vtkTypeBool IsTypeOf(const char *type) 													\
  { 																								\
    if ( !strcmp(thisClassName,type) ) 																\
    { 																								\
      return 1; 																					\
    } 																								\
    return superclass::IsTypeOf(type); 																\
  } 																								\
  vtkTypeBool IsA(const char *type) override 														\
  { 																								\
    return this->thisClass::IsTypeOf(type);															\
  } 																								\
  static thisClass* SafeDownCast(vtkObjectBase *o,bool showError=true) 								\
  { 																								\
    if ( o && o->IsA(thisClassName) ) 																\
    { 																								\
      return static_cast<thisClass *>(o); 															\
    } 																								\
	if(showError==true){																			\
		stkGenericWarningMacro("Failure to SafeDownCast to " << thisClassName << 						\
				" likely that the object was not originally derived from this class")				\
	}																								\
    return nullptr;																					\
  } 																								\
  VTK_NEWINSTANCE instanceType *NewInstance() const 												\
  { 																								\
    return instanceType::SafeDownCast(this->NewInstanceInternal()); 								\
  }

//! Custom version of stkAbstractTypeMacro
#define stkAbstractTypeMacro(thisClass,superclass) 													\
  stkAbstractTypeMacroWithNewInstanceType(thisClass, superclass, thisClass, #thisClass) 			\
  public:

//! Custom version of vtkTypeMacro
#define stkTypeMacro(thisClass,superclass) 															\
  stkAbstractTypeMacro(thisClass, superclass) 														\
  protected: 																						\
  vtkObjectBase *NewInstanceInternal() const override 												\
  { 																								\
    return thisClass::New(); 																		\
  } 																								\
  public:




#endif /* CPP_COMMON_STKMACROS_HPP_ */

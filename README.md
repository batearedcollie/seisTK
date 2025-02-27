
SeisTK
======

Introduction
------------

The purpose of seisTK is to provide data types and binds which allow representation of seismic data in VTK.
In particular this adds three pieces of functionality 

*vtkHyperCube* provides the ability to store and manipulate grids with more than 3 dimensions. 

*vtkTraceHeader* provides storage of seismic trace headers. 

*vtkTracePanelData* provides storage of seismic trace data. 

In it's current form the project is really designed for those who have a low level understanding of VTK and python. 


Installation
------------

### Prerequisites

* VTK (compiled with Python bindings) [https://github.com/Kitware/VTK]

* ObsPy [https://github.com/obspy/obspy] or ObsLn [https://github.com/batearedcollie/obsln]

* Madagascar (for the tests) [https://github.com/ahay/src/] - compiled with python/ m8r bindings

It will build succesfully without ObsPy and Madagascar, however python functionality will be compromised and some tests wil fail
The Madagascar extesions can be disabled by supplying the argument `-DSEISTK_DISABLE_MADAGASCAR_EXT:BOOL=TRUE`
in cmake. 

###  Building 

 
Use Cmake to configure the build

    $ cmake path/to/source

Then *make* and *make install*

    $ make 

    $ make install 

### Additional targets

In the style of *cmake* you can run

    $ make help

To get a list of potential targets. Notable items include.

* *doc* - Doxygen documentation

* *make run_vtkHyperCubeTest* - runs the test program *vtkHyperCubeTest*

* *test* - run the automated testing


Usage
-----

See:

* cpp/Examples
* stk/Examples


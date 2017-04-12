
SeisTK
======

Introduction
------------

The purpose of seisTK is to provide data types and binds which allow representation of seismic data in VTK.
In particular this adds two pieces of functionality 

*vtkHyperCube* provides the ability to store and manipulate grids with more than 3 dimensions. 

*vtkTracePanelData* provides storage of seismic trace data. 

In it's current form the project is really designed for those who have a low level understanding of VTK and python. 


Installation
------------

### Prerequisites

* Boost and Boost Python

* VTK (compiled with Python bindings)

* ObsPy (for the tests)

###  Building 

 
* Use Cmake to configure the build

    $ cmake path/to/source

* Then *make* and *make install*

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
* python/Examples


# DynELA Finite Element Code v.4.0

<img src="Documentation/Figures/Taylor3D.png" width="50%" alt="Taylor impact test" align="right"/>

This repository contains the DynELA Finite Element Code v.4.0. The DynELA Finite Element Code is an Explicit FEM code written in C++ using a Python's interface for creating the Finite Element Models. This is a new version of the early proposed v.2 code written between 1996 and 2010. The previous version has been included into the CAE Linux distribution some year ago and the corresponding work has been published in some Scientific Journals (Advances in Engineering Software, Journal of Computational and Applied Mathematics;...). The aim of v.4.0 is to provide a new version of the code with enhancements concerning the constitutive laws, a new programming interface based on Python 3 formalism, along with some documentation.

## Download and installation procedure:

The DynELA FEM code is developped under Linux (an Ubuntu 20.04 LTS is currently used for the development).

### Prerequisites:

Compilation of the DynELA Finite Element Code requires several libraries.

Generation of Makefiles for the compilation of the DynELA Finite Element Code is based on the use of the CMake tool. CMake is a cross-platform, open-source build system generator. Under ubuntu it can be installed with the following command:

	sudo apt install cmake

DynELA is written in C++ and Python 3 therefore it needs a C++ compiler and some Python 3 libraries. Under ubuntu this can be installed with the following command:

	sudo apt install build-essential swig zlib1g-dev liblapacke-dev python3-dev

It also needs some Python 3 modules to run properly and at least numpy and matplotlib:

	sudo apt install python3-numpy python3-matplotlib texlive dvipng texlive-latex-extra texlive-fonts-recommended

### Download and compilation procedure:

Downloading of the source code from this **git** repository can be done using a **git clone** command. Then, compilation and installation of the software into a sub-directory can be done using the following procedure:

	cd DynELA
	mkdir Build
	cd Build
	cmake ../Sources
	make

After downloading and compilation, there is no need to install the executable or something similar to use the FEM code. You just have to modify the **.bashrc** file and add the following lines where _path_to_DynELA_ points to the top directory of your DynELA Finite Element Code installation:

	export DYNELA="path_to_DynELA"
	export PATH=$PATH:$DYNELA/bin
	export DYNELA_BIN=$DYNELA/Build/bin
	export DYNELA_LIB=$DYNELA/Build/lib
	export PYTHONPATH="$DYNELA_BIN:$PYTHONPATH"
	export PYTHONPATH="$DYNELA_LIB:$PYTHONPATH"
	export LD_LIBRARY_PATH=$DYNELA_LIB:$LD_LIBRARY_PATH

## Testing and usage:

Testing of the installation can be done by running one of the provided samples. All samples of the DynELA Finite Element Code are located into the sub-directories of the Samples folder. Running a simulation is done using the following command in one of the Samples sub-directories:

	python sample.py

Running the tests in the Samples directories can be done through the Makefiles contained in the Samples directories. Benchmark tests can be run from any subdirectory of the Sample folder using the following command:

	make

## Postprocessing:

The DynELA Finite Element Code now has a class for direct export of contourplot results using SVG vectorial format for a 2D or 3D mesh and time-history curves through the Python command interface. See the documentation for all instructions concerning SVG and time-history outputs and the examples included in the Samples directories.

The DynELA FEM code can generate VTK files for the results. I'm using the Paraview postprocessor to visualize those results. Paraview is available here: https://www.paraview.org/download

***
Olivier Pantalé  
Full Professor of Mechanics  
email : olivier.pantale@enit.fr

Laboratoire Génie de Production  
Ecole Nationale d'Ingénieurs de Tarbes  
Université de Toulouse  
47 Avenue d'Azereix - BP 1629  
65016 TARBES - CEDEX - FRANCE

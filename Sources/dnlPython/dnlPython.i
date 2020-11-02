/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/

%feature("autodoc","1");
%include "std_string.i"
%include "cpointer.i"
%pointer_functions(double ,doubleP)
%pointer_functions(int ,intP)
%pointer_functions(long ,longP)

%module dnlPython
%warnfilter(321) RampFunction::set;
%warnfilter(402);

%include "dnlKernel.inc"
%include "dnlMaths.inc"
%include "dnlMaterials.inc"
%include "dnlElements.inc"
%include "dnlBC.inc"
%include "dnlFEM.inc"

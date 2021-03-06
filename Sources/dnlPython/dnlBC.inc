/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 4.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%{
  #include "Boundary.h"
  #include "BoundaryCondition.h"
  #include "BoundaryAcceleration.h"
  #include "BoundaryConvection.h"
  #include "BoundaryDisplacement.h"
  #include "BoundaryFlux.h"
  #include "BoundaryForce.h"
  #include "BoundaryRestrain.h"
  #include "BoundarySpeed.h"
  #include "BoundaryTemperature.h"
%}

%include "Boundary.h"
%include "BoundaryCondition.h"
%include "BoundaryAcceleration.h"
%include "BoundaryConvection.h"
%include "BoundaryDisplacement.h"
%include "BoundaryFlux.h"
%include "BoundaryForce.h"
%include "BoundaryRestrain.h"
%include "BoundarySpeed.h"
%include "BoundaryTemperature.h"

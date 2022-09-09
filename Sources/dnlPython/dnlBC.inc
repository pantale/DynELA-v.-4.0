/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
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

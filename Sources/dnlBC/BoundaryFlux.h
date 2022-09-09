/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlBC_BoundaryFlux_h__
#define __dnlBC_BoundaryFlux_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryFlux
//
// Used to manage BoundaryFluxs in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryFlux : public Boundary
{

private:
  double _flux;

public:
  // constructeurs
  BoundaryFlux(char *newName = NULL);
  BoundaryFlux(double newFlux);
  BoundaryFlux(const BoundaryFlux &X);
  ~BoundaryFlux();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  // fonctions membres
  long getType() { return Boundary::Flux; }
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  double getValue();
  void setValue(double val);
};

#endif

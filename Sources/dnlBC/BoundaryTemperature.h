/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlBC_BoundaryTemperature_h__
#define __dnlBC_BoundaryTemperature_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryTemperature
//
// Used to manage BoundaryTemperatures in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryTemperature : public Boundary
{

private:
  double _temperature;

public:
  // constructeurs
  BoundaryTemperature(char *newName = NULL);
  BoundaryTemperature(double newTemperaure);
  BoundaryTemperature(const BoundaryTemperature &X);
  ~BoundaryTemperature();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  // fonctions membres
  long getType()
  {
    return Boundary::Temperature;
  }
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  double getValue();
  void setValue(double newTemperaure);
};
#endif

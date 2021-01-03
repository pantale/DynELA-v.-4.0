/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlBC_BoundaryConvection_h__
#define __dnlBC_BoundaryConvection_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryConvection
//
// Used to manage BoundaryConvections in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryConvection : public Boundary
{

public:
  double _hc;
  double _Tf;

public:
  // constructeurs
  BoundaryConvection(char *newName = NULL);
  BoundaryConvection(double newhc, double newTf);
  BoundaryConvection(const BoundaryConvection &X);
  ~BoundaryConvection();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  // fonctions membres
  long getType() { return Boundary::Convection; }
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(double newhc, double newTf);
};

#endif

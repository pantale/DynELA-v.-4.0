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

#ifndef __dnlBC_BoundaryRestrain_h__
#define __dnlBC_BoundaryRestrain_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryRestrain
//
// Used to manage BoundaryRestrains in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryRestrain : public Boundary
{

private:
  Vec3D _restrain;

public:
  // constructeurs
  BoundaryRestrain(char *newName = NULL);
  BoundaryRestrain(const Vec3D &newRestrain);
  BoundaryRestrain(const BoundaryRestrain &X);
  ~BoundaryRestrain();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newRestrain);
  void setValue(double restrainX, double restrainY, double restrainZ);

  // fonctions membres
  long getType()
  {
    return Boundary::Restrain;
  }
};

#endif

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

#ifndef __dnlBC_BoundaryForce_h__
#define __dnlBC_BoundaryForce_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryForce
//
// Used to manage BoundaryForces in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryForce : public Boundary
{

private:
  Vec3D _force;

public:
  // constructeurs
  BoundaryForce(char *newName = NULL);
  BoundaryForce(const Vec3D &newForce);
  BoundaryForce(const BoundaryForce &X);
  ~BoundaryForce();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  // fonctions membres
  long getType()
  {
    return Boundary::Force;
  }
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newForce);
  void setValue(double forceX, double forceY, double forceZ);
};

#endif

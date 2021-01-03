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

#ifndef __dnlBC_BoundaryAcceleration_h__
#define __dnlBC_BoundaryAcceleration_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryAcceleration
//
// Used to manage BoundaryAcceleration in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryAcceleration : public Boundary
{

private:
  Vec3D _acceleration;

public:
  // constructeurs
  BoundaryAcceleration(char *newName = NULL);
  BoundaryAcceleration(const Vec3D &newAcceleration);
  BoundaryAcceleration(const BoundaryAcceleration &X);
  ~BoundaryAcceleration();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  Vec3D getValue();
  void setValue(const Vec3D &newAcceleration);
  void setValue(double accX, double accY, double accZ);
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);

  // fonctions membres
  long getType()
  {
    return Boundary::Acceleration;
  }
};

#endif

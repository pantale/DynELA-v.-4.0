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

#ifndef __dnlBC_BoundaryDisplacement_h__
#define __dnlBC_BoundaryDisplacement_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryDisplacement
//
// Used to manage BoundaryDisplacements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryDisplacement : public Boundary
{

private:
  Vec3D _displacement;

public:
  // constructeurs
  BoundaryDisplacement(char *newName = NULL);
  BoundaryDisplacement(const Vec3D &newDisplacement);
  BoundaryDisplacement(const BoundaryDisplacement &X);
  ~BoundaryDisplacement();

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
  void setValue(const Vec3D &newDisplacement);
  void setValue(double dispX, double dispY, double dispZ);

  // fonctions membres
  long getType()
  {
    return Boundary::Displacement;
  }
};
#endif
